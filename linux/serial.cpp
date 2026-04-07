#include <string.h> 
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdexcept>
#include <thread>

#include "framework/nativescheduler.h"

#include "core/strstream.h"
#include "core/flowctrl.h"
#include "core/flowctrlwin.h"
#include "hal/system.h"
#include "hal/serial.h"
#include "hal/diag.h"

namespace piece {
namespace hal {

class SerialPort {
public:
    SerialPort(const variant& settings) {
        deviceName = (const char*)settings["device"];
        baudRate = convertBaudRate((int)settings["rate"]);
        fd = -1;
    }

    ~SerialPort() {
        disconnect();
    }

    void connect() {
        if (fd >= 0)
            return;
        fd = open(deviceName.c_str(), O_RDWR | O_NOCTTY | O_SYNC | O_NDELAY);
        if (fd < 0)
            throw std::runtime_error("Failed to open serial port: " + std::string(strerror(errno)));
        configure(baudRate);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        char buffer[1024];
        int bytes = 0;
        for(;;) {
            auto tmp = read(fd, buffer, 1024);
            hal::System::wait_ms(1);
            if (tmp == 0)
                break;
            bytes += tmp;
        }
    }

    void disconnect() {
        if (fd >= 0)
            close(fd);
        fd = -1;
    }

    int writeData(const uint8_t* buffer, int size, int timeout) {
        ssize_t result = 0;
        do {
            auto t1 = System::uptime_ms();
            result = write(fd, buffer, size);
            timeout -= System::uptime_ms() - t1;
            if (result < 0) {
                if (errno != EAGAIN)
                    throw std::runtime_error("Write failed: " + std::string(strerror(errno)));
                result = 0;
            }
        }
        while(result != size && timeout > 0);
        return result;
    }

    int readData(uint8_t* buffer, int size, int timeout) {
        ssize_t result = read(fd, buffer, size);
        if (result < 0) {
            if (errno != EAGAIN)
                throw std::runtime_error("Write failed: " + std::string(strerror(errno)));
            result = 0;
        }
        return result;
    }

    bool test() {
        struct termios tty;
        if (tcgetattr(fd, &tty) != 0) {
            return false;
        }
        return true;
    }

private:
    int fd;
    std::string deviceName;
    int baudRate;

    void configure(int baudrate) {
        struct termios tty;
        memset(&tty, 0, sizeof(termios));
        cfsetospeed(&tty, baudrate);
        cfsetispeed(&tty, baudrate);
        tty.c_cflag &= ~PARENB;                         // Clear parity bit, disabling parity (N)
        tty.c_cflag &= ~CSTOPB;                         // Clear stop field, only one stop bit (1)
        tty.c_cflag &= ~CSIZE;                          // Clear current data size setting
        tty.c_cflag |= CS8;                             // 8 data bits (8)
        tty.c_cflag &= ~CRTSCTS;                        // Disable RTS/CTS hardware flow control
        tty.c_cflag |= CREAD | CLOCAL;                  // Turn on READ & ignore ctrl lines (CLOCAL = 1)
        tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw input
        tty.c_iflag &= ~(IXON | IXOFF | IXANY);         // Turn off s/w flow ctrl
        tty.c_oflag &= ~OPOST;                          // Raw output
        if (tcsetattr(fd, TCSANOW, &tty) != 0) {
            throw std::runtime_error("tcsetattr failed: " + std::string(strerror(errno)));
        }
    }

    speed_t convertBaudRate(int baudrate) {
        switch (baudrate) {
            case 9600: return B9600;
            case 19200: return B19200;
            case 38400: return B38400;
            case 57600: return B57600;
            case 115200: return B115200;
            default:
                throw std::invalid_argument("Unsupported baud rate");
        }
    }
};

class SerialDevice : public ISerialDevice {
friend void registerSerialDevice();
public:
    SerialDevice()
        : rxCallbackParam(nullptr), portStats{0,0,0,0}
    {
    }

    ~SerialDevice() {
        if (flowCtrl != nullptr) {
            delete flowCtrl;
            flowCtrl = nullptr;
        }
        if (serialPort != nullptr) {
            delete serialPort;
            serialPort = nullptr;
        }
    }

    int initiate(variant settings) override {
        try {
            serialPort = new SerialPort(settings);
            auto flowctrl = *settings["flowctrl"];
            if (flowctrl.type() == v_type::vstring && flowctrl == "sliding-window")
                flowCtrl = new net::WindowFlowCtrl(settings);
            else
                flowCtrl = new net::FlowControl(settings);
            flowCtrl->setCallbacks(
                [](void* param, const uint8_t* data, int size) {
                    auto _this = (SerialDevice*)param;
                    _this->rxCallback(data, size, _this->rxCallbackParam);
                },
                [](void* param, uint8_t* data, int size, int timeout) -> int {
                    auto _this = (SerialDevice*)param;
                    auto result = _this->serialPort->readData(data, size, timeout);
                    //if (result) {
                    //    hal::Diag::log("READ ", result);
                    //    hal::Diag::write(data, result);
                    //}
                    return result;
                },
                [](void* param, const uint8_t* data, int size, int timeout) -> int {
                    auto _this = (SerialDevice*)param;
                    auto result = _this->serialPort->writeData(data, size, timeout);
                    //if (size) {
                    //    hal::Diag::log("WRITE ", size, "/", result);
                    //    hal::Diag::write(data, size);
                    //}
                    return result;
                },
                this);
            NativeScheduler::inst().activate("hal-serial", this, &SerialDevice::process, 1, true);
        }
        catch(...) {
            if (serialPort != nullptr) {
                delete serialPort;
                serialPort = nullptr;
            }
            if (flowCtrl != nullptr) {
                delete flowCtrl;
                flowCtrl = nullptr;
            }
            return -1;
        }
        return 0;
    }

    const DeviceInfo& info() override {
        return deviceInfo;
    }

    void destroy() override {
        NativeScheduler::inst().deactivate("hal-serial");
        delete this;
    }

    int connect(recv callback, void* param) override {
        try {
            if (serialPort != nullptr && flowCtrl != nullptr) {
                rxCallback = callback;
                rxCallbackParam = param;
                serialPort->connect();
                flowCtrl->start();
            }
        }
        catch(...) {
            return -1;
        }
        return 0;
    }

    int disconnect() override {
        if (serialPort)
            serialPort->disconnect();
        if (flowCtrl)
            flowCtrl->reset();
        return 0;
    }

    bool isConnected() override {
        return serialPort && flowCtrl && serialPort->test() && flowCtrl->connected();
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
    SerialPort* serialPort;
    net::FlowControl* flowCtrl;
    static DeviceInfo deviceInfo;

    void send(const char* str, int size = 0) {
        if (size == 0)
            size = strlen(str);
        send((const uint8_t*)str, size);
    }

    bool process() {
        if (serialPort && flowCtrl) {
            try {
                while(flowCtrl->process());
            }
            catch(...) {
            }
        }
        return true;
    }
};

DeviceInfo SerialDevice::deviceInfo = {
    nullptr, nullptr,
    DeviceType::serial,
    DeviceClass::transmission,
    "serial",
    "linux",
    "Generic serial device on Linux platform"
};

void registerSerialDevice() {
    DeviceFactory::registerDevice(
        SerialDevice::deviceInfo,
        [](const DeviceInfo& info) -> IDevice* {
            return new SerialDevice();
        }
    );
}

}
}
