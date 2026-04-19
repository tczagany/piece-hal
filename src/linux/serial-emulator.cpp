#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <signal.h>

#include "piece.h"

#include "core/flowctrl.h"
#include "core/flowctrlwin.h"

#include "hal/serial.h"

namespace piece {
namespace hal {

class SerialEmulator {
public:
    SerialEmulator(variant settings)
        : fdPeer(-1), fdSelf(-1)
    {
        peerName = selfName = (const char*)settings["device"];
        if (settings["endpoint"] == "A") {
            selfName += "-A";
            peerName += "-B";
        }
        else {
            selfName += "-B";
            peerName += "-A";
        }
        umask(0);
        if (mknod(peerName.c_str(), S_IFIFO | 0660, 0) == -1) {
            if (errno != EEXIST)
                throw std::runtime_error("Failed to open serial emulator 'peer' file");
        }
        if (mknod(selfName.c_str(), S_IFIFO | 0660, 0) == -1) {
            if (errno != EEXIST)
                throw std::runtime_error("Failed to open serial emulator 'self' file");
        }
        signal(SIGPIPE, SIG_IGN);
    }

    ~SerialEmulator() {
        disconnect();
    }

    bool connect() {
        if (fdPeer < 0)
            fdPeer = open(peerName.c_str(), O_RDONLY|O_NONBLOCK);
        if (fdSelf < 0)
            fdSelf = open(selfName.c_str(), O_WRONLY|O_NONBLOCK);
        return fdSelf >= 0 && fdPeer >= 0;
    }

    void disconnect() {
        if (fdSelf >= 0)
            close(fdSelf);
        if (fdPeer >= 0)
            close(fdPeer);
        fdSelf = fdPeer = -1;
    }

    int writeData(const uint8_t* buffer, int size, int) {
        ssize_t done = write(fdSelf, buffer, size);
        if (done <= 0) {
            return 0;
        }
        fsync(fdSelf);
        return done;
    }

    int readData(uint8_t* buffer, int size, int) {
        ssize_t done = read(fdPeer, buffer, size);
        if (done <= 0) {
            return 0;
        }
        return done;
    }

    bool test() {
        return fdSelf >= 0 && fdPeer >= 0;
    }

private:
    int fdSelf;
    std::string selfName;
    int fdPeer;
    std::string peerName;
};

class SerialEmulatorDevice : public ISerialDevice {
friend void registerSerialEmulatorDevice();
public:
    SerialEmulatorDevice()
        : rxCallbackParam(nullptr), portStats{0,0,0,0}
    {
    }

    ~SerialEmulatorDevice() {
        if (flowCtrl != nullptr) {
            delete flowCtrl;
            flowCtrl = nullptr;
        }
        if (serialPortEmulator != nullptr) {
            delete serialPortEmulator;
            serialPortEmulator = nullptr;
        }
    }

    int initiate(variant settings) override {
        try {
            serialPortEmulator = new SerialEmulator(settings);
            auto flowctrl = *settings["flowctrl"];
            if (flowctrl.type() == v_type::vstring && flowctrl == "sliding-window")
                flowCtrl = new net::WindowFlowCtrl(settings);
            else
                flowCtrl = new net::FlowControl(settings);
            flowCtrl->setCallbacks(
                [](void* param, const uint8_t* data, int size) {
                    auto _this = (SerialEmulatorDevice*)param;
                    _this->rxCallback(data, size, _this->rxCallbackParam);
                },
                [](void* param, uint8_t* data, int size, int timeout) -> int {
                    auto _this = (SerialEmulatorDevice*)param;
                    auto res = _this->serialPortEmulator->readData(data, size, timeout);
                    //if (res) {
                    //    hal::Diag::log("READ");
                    //    hal::Diag::write(data, res);
                    //}
                    return res;
                },
                [](void* param, const uint8_t* data, int size, int timeout) -> int {
                    auto _this = (SerialEmulatorDevice*)param;
                    //if (size) {
                    //    hal::Diag::log("WRITE");
                    //    hal::Diag::write(data, size);
                    //}
                    return _this->serialPortEmulator->writeData(data, size, timeout);
                },
                this);
                NativeScheduler::inst().activate("hal-e-serial", this, &SerialEmulatorDevice::process, 1, true);
        }
        catch(...) {
            if (serialPortEmulator != nullptr) {
                delete serialPortEmulator;
                serialPortEmulator = nullptr;
            }
            return -1;
        }
        return 0;
    }

    void destroy() override {
        NativeScheduler::inst().deactivate("hal-e-serial");
        delete this;
    }

    const DeviceInfo& info() override {
        return deviceInfo;
    }

    int connect(recv callback, void* param) override {
        try {
            if (serialPortEmulator && flowCtrl) {
                if (serialPortEmulator->connect()) {
                    rxCallback = callback;
                    rxCallbackParam = param;
                    flowCtrl->start();
                }
            }
        }
        catch(...) {
            return -1;
        }
        return 0;
    }

    int disconnect() override {
        if (serialPortEmulator) {
            serialPortEmulator->disconnect();
            flowCtrl->reset();
        }
        return 0;
    }

    bool isConnected() override {
        return serialPortEmulator && flowCtrl && serialPortEmulator->test() && flowCtrl->connected();
    }

    int send(const uint8_t* data, int size) override {
        if (flowCtrl == nullptr)
            return 0;
        auto result = flowCtrl->write(data, size);
        if (result)
            flowCtrl->process();
        return result;
    }

    const statistics& stats() override {
        return portStats;
    }

    void writeTxt(bool val) override {
        if (val)
            send("true");
        else
            send("false");
    }

    void writeTxt(int num) override {
        ostringstream os(32);
        os << num;
        send(os.str());
    }

    void writeTxt(float num, int digits = 2) override {
        ostringstream os(32);
        os << ostringstream::Width{digits};
        os << num;
        send(os.str());
    }

    void writeTxt(const char* str) override {
        send(str);
    }

    void writeBin(int num) override {
        send((const uint8_t*)&num, 4);
    }

    void writeBin(uint32_t num) override {
        send((const uint8_t*)&num, 4);
    }

    void writeBin(const char* str) override {
        send(str);
    }

    void writeBin(const uint8_t* buffer, int size) override {
        send(buffer, size);
    }

    void enumeratePorts(void* param, portEnumCallback cb) override {
    }

protected:
    recv rxCallback;
    void *rxCallbackParam;
    statistics portStats;
    SerialEmulator* serialPortEmulator;
    net::FlowControl* flowCtrl;
    static DeviceInfo deviceInfo;

    void send(const char* str, int size = 0) {
        if (size == 0)
            size = strlen(str);
        send((const uint8_t*)str, size);
    }

    bool process() {
        if (serialPortEmulator && flowCtrl) {
            try {
                flowCtrl->process();
            }
            catch(...) {
                disconnect();
            }
        }
        return true;
    }
};

DeviceInfo SerialEmulatorDevice::deviceInfo = {
    nullptr, nullptr,
    DeviceType::serial,
    DeviceClass::transmission,
    "serial-emu",
    "piece",
    "Generic serial device emulator on Linux platform"
};

void registerSerialEmulatorDevice() {
    DeviceFactory::registerDevice(
        SerialEmulatorDevice::deviceInfo,
        [](const DeviceInfo& info) -> IDevice* {
            return new SerialEmulatorDevice();
        }
    );
}

}
}
