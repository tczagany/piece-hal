#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include <stdexcept>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <pthread.h>
#include <vector>
#include <string.h> 
#include <functional>
#include <atomic>

#include "piece.h"

#include "core/nettypes.h"
#include "hal/wifi.h"
#include "hal/interface.h"

namespace piece {
namespace hal {

static const int portsCount = 200;

class PortAllocator {
protected:
    struct Node {
        uint32_t id;
        uint16_t port;
        bool monitor;
    };

    struct SharedArea {
        uint32_t version;
        uint32_t pids[portsCount];
        Node nodes[portsCount];
        std::atomic_flag lock = ATOMIC_FLAG_INIT;
    };

public:
    PortAllocator(int port, const char* name, uint32_t nodeId)
        : firstPort(port), allocatedPort(0),
          sharedFD(-1), version(0), sharedPtr(nullptr)
    {
        auto initialOpen = false;
        sharedFD = shm_open(name, O_CREAT|O_EXCL|O_RDWR, 0666);
        if (sharedFD >= 0) {
            initialOpen = true;
            ftruncate(sharedFD, sizeof(SharedArea));
        }
        else {
            sharedFD = shm_open(name, O_RDWR, 0666);
        }
        if (sharedFD < 0)
            throw std::logic_error{"memory sharing error 1"};
        sharedPtr = (SharedArea*)mmap(NULL, sizeof(SharedArea),
            PROT_READ | PROT_WRITE, MAP_SHARED, sharedFD, 0);
        if (sharedPtr == MAP_FAILED) {
            close(sharedFD);
            throw std::logic_error{"memory sharing error 2"};
        }
        if (initialOpen) {
            new(sharedPtr)SharedArea();
            sharedPtr->lock.clear();
        }
        clearUnusedPIDs();
        while (sharedPtr->lock.test_and_set());
        auto pid = getpid();
        int ownIndex = -1;
        int firstUnused = -1;
        for(int i = 0; i < portsCount; i++) {
            if (firstUnused == -1 && sharedPtr->pids[i] == 0)
                firstUnused = i;
            if (sharedPtr->pids[i] == pid)
                ownIndex = i;
        }
        if (ownIndex == -1 && firstUnused != -1) {
            sharedPtr->nodes[firstUnused].port = port;
            sharedPtr->nodes[firstUnused].id = nodeId;
            sharedPtr->nodes[firstUnused].monitor = false;
            sharedPtr->pids[firstUnused] = pid;
        }
        sharedPtr->version++;
        allocatedPort = port;
        version = sharedPtr->version;
        sharedPtr->lock.clear();
    }

    ~PortAllocator() {
        while (sharedPtr->lock.test_and_set());
        auto pid = getpid();
        for(int i = 0; i < portsCount; i++) {
            if (sharedPtr->pids[i] == pid) {
                sharedPtr->nodes[i].port = 0;
                sharedPtr->nodes[i].id = 0;
                sharedPtr->nodes[i].monitor = false;
                sharedPtr->pids[i] = 0;
            }
        }
        sharedPtr->version++;
        sharedPtr->lock.clear();
        munmap(sharedPtr, sizeof(SharedArea));
        close(sharedFD);
    }

    Node* begin() {
        if (sharedPtr->version != version) {
            nodes.clear();
            for(int i = 0; i < portsCount; i++) {
                if (sharedPtr->pids[i] != 0 &&
                    sharedPtr->nodes[i].port != allocatedPort)
                {
                    nodes.push_back(sharedPtr->nodes[i]);
                }
            }
            version = sharedPtr->version;
        }
        return &*nodes.begin();
    }

    Node* end() {
        return &*nodes.end();
    }

    bool ready() {
        return allocatedPort != 0;
    }

    void clearUnusedPIDs() {
        nodes.clear();
        for(int i = 0; i < portsCount; i++) {
            auto pid = &sharedPtr->pids[i];
            if (*pid != 0) {
                if (!(kill(*pid, 0) == 0 || errno != ESRCH)) {
                    *pid = 0;
                    sharedPtr->nodes[i].id = 0;
                    sharedPtr->nodes[i].port = 0;
                    sharedPtr->nodes[i].monitor = false;
                }
                else if (sharedPtr->nodes[i].port != allocatedPort) {
                    nodes.push_back(sharedPtr->nodes[i]);
                }
            }
        }
    }

    void activateMonitoring() {
    }

private:
    int firstPort;
    int sharedFD;
    uint32_t version;
    SharedArea *sharedPtr;
    uint16_t allocatedPort;
    std::vector<Node> nodes;
};

class UDPIOControl {
public:
    using rx_callback = std::function<void(const uint8_t*, int, void*)>;

public:
    UDPIOControl(const variant& settings)
        : socketId(-1), nextUpdate(0), portAllocator(nullptr),
          nodeId(0), taskId(0), rxCallback(nullptr), configured(false), connDelay(0)
    {
        auto ert = (const char*)settings.toString();
        netPort = (int)settings["port"];
        netName = (const char*)settings["ssid"];
        nodeId = (int)settings["node-id"];
        auto asd = settings.find("conn-delay");
        if (!asd.empty())
            connDelay = (int)settings["conn-delay"];
    }

    ~UDPIOControl() {
        disconnect();
    }

    int connect(rx_callback callback) {
        if (connected())
            return 0;
        if (netPort != 0 && connDelay <= 0) {
            socketId = socket(AF_INET, SOCK_DGRAM, 0);
            if (socketId >= 0) {
                int yes = 1;
                auto ret = setsockopt(socketId, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes));
                if (ret == -1)
                    throw std::logic_error("socket opening error");
            }
            else
                throw std::logic_error("socket creation error");
            if (!bindUDPPort())
                throw std::logic_error("port scan or socket binding error");
            rxCallback = callback;
            configured = true;
        }
        if (taskId == 0)
            taskId = NativeScheduler::inst().activate("hal-eb-wifi", this, &UDPIOControl::process, 1, true);
        return 0;
    }

    int disconnect() {
        if (!connected())
            return 0;
        NativeScheduler::inst().deactivate("hal-eb-wifi");
        taskId = 0;
        if (portAllocator)
            delete portAllocator;
        if (socketId >= 0)
            close(socketId);
        socketId = -1;
        configured = false;
        portAllocator = nullptr;
        return 0;
    }

    bool connected() {
        return configured;
    }

    void write(const uint8_t* msg, int size) {
        if (connected()) {
            auto header = (net::MessageHeader*)msg;
            sockaddr_in broadcast_addr;
            memset((void*)&broadcast_addr, 0, sizeof(sockaddr_in));
            broadcast_addr.sin_family = AF_INET;
            broadcast_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            for(auto node : *portAllocator) {
                broadcast_addr.sin_port = htons(node.port);
                auto result = sendto(socketId, msg, size, 0,
                    (struct sockaddr*) &broadcast_addr, sizeof(sockaddr_in));
                if (result < 0 || result != size) {
                    configured = false;
                    throw std::logic_error("UDP send error");
                }
            }
        }
    }

    void activateMonitoring() {
    }

protected:
    int socketId;
    int netPort;
    int connDelay;
    uint32_t nodeId;
    uint32_t taskId;
    std::string netName;
    PortAllocator* portAllocator;
    uint32_t nextUpdate;
    rx_callback rxCallback;
    bool configured;

    bool bindUDPPort() {
        for(int i = 9000; i < 10000; i++) {
            auto len = sizeof(sockaddr_in);
            sockaddr_in addr;
            memset(&addr, 0, len);
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(i);
            auto ret = bind(socketId, (sockaddr*)&addr, len);
            if (ret >= 0) {
                portAllocator = new PortAllocator(i, netName.c_str(), nodeId);
                return true;
            }
        }
        return false;
    }

    bool process() {
        if (connDelay == 0 && configured) {
            auto now = hal::System::uptime_ms();
            if (now > nextUpdate) {
                nextUpdate = now + hal::System::random(2000, 3000);
                portAllocator->clearUnusedPIDs();
            }
            uint8_t buffer[1024];
            int size = recv(socketId, buffer, 1024, MSG_DONTWAIT);
            if (size > 0 && rxCallback != nullptr)
                rxCallback(buffer, size, this);
            if (size < 0 && errno != EAGAIN)
                configured = false;
        }
        else if (connDelay > 0) {
            connDelay--;
            if (connDelay == 0)
                hal::System::wait_ms(1500);
        }
        return true;
    }
};

class WiFiDeviceEmulator : public IWiFiDevice {
friend void registerWiFiEmulatorDevice();
public:
    WiFiDeviceEmulator()
        : udpIO(nullptr), rxCallbackParam(nullptr), wifiStats{0,0,0,0}
    {
    }

    ~WiFiDeviceEmulator() {
        if (udpIO == nullptr) {
            delete udpIO;
            udpIO = nullptr;
        }
    }

    int initiate(variant settings) override {
        if (udpIO != nullptr)
            return -1;
        try {
            udpIO = new UDPIOControl(settings);
        }
        catch(...) {
            udpIO = nullptr;
            return -1;
        }
        return 0;
    }

    const DeviceInfo& info() override {
        return deviceInfo;
    }

    void destroy() override {
        delete this;
    }

    int connect(recv callback, void* param) override {
        if (udpIO == nullptr)
            return -1;
        try {
            udpIO->connect([=](const uint8_t* msg, int size, void*) {
                rxCallbackParam = param;
                wifiStats.rxMsgCount += 1;
                wifiStats.rxBytesCount += size;
                callback(msg, size, rxCallbackParam);
            });
        }
        catch(...) {
            return -1;
        }
        return 0;
    }

    int disconnect() override {
        if (udpIO == nullptr)
            return -1;
        if (!udpIO->connected())
            return -1;
        udpIO->disconnect();
        return 0;
    }

    bool isConnected() override {
        return udpIO != nullptr && udpIO->connected();
    }

    int send(const uint8_t* data, int size) override {
        if (udpIO == nullptr)
            return -1;
        try {
            udpIO->write(data, size);
            wifiStats.txMsgCount += 1;
            wifiStats.txBytesCount += size;
        }
        catch(...) {
            return -1;
        }
        return size;
    }

    const statistics& stats() override {
        return wifiStats;
    }

    void setOption(uint8_t) override {
        udpIO->activateMonitoring();
    }

    void startSSIDQuery(netEnumCallback cb, void* param) override {
    }

    void cancelSSIDQuery() override {
    }

protected:
    UDPIOControl *udpIO;
    void *rxCallbackParam;
    statistics wifiStats;
    static DeviceInfo deviceInfo;
};

DeviceInfo WiFiDeviceEmulator::deviceInfo = {
    nullptr, nullptr,
    DeviceType::wifi_emulator,
    DeviceClass::transmission,
    "wifi-broadcast-emu",
    "piece",
    "Emulated device to broadcast packets on Linux platform"
};

void registerWiFiEmulatorDevice() {
    DeviceFactory::registerDevice(
        WiFiDeviceEmulator::deviceInfo,
        [](const DeviceInfo& info) -> IDevice* {
            return new WiFiDeviceEmulator();
        }
    );
}

}
}
