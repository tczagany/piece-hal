#include "hal/wifi.h"
#include "hal/interface.h"

namespace piece {
namespace hal {

class WiFiDevice : public IWiFiDevice {
friend void registerWiFiDevice();
public:
    WiFiDevice() : rxCallbackParam(nullptr), wifiStats{0,0,0,0}
    {
    }

    int initiate(variant settings) override {
        return 0;
    }

    const DeviceInfo& info() override {
        return deviceInfo;
    }

    void destroy() override {
        delete this;
    }

    int connect(recv callback, void* param) override {
        return 0;
    }

    int disconnect() override {
        return 0;
    }

    bool isConnected() override {
        return false;
    }

    int send(const uint8_t* data, int size) override {
        return size;
    }

    const statistics& stats() override {
        return wifiStats;
    }

    void startSSIDQuery(netEnumCallback cb, void* param) override {
    }

    void cancelSSIDQuery() override {
    }

protected:
    void *rxCallbackParam;
    statistics wifiStats;
    static DeviceInfo deviceInfo;
};

DeviceInfo WiFiDevice::deviceInfo = {
    nullptr, nullptr,
    DeviceType::wifi,
    DeviceClass::transmission,
    "wifi-broadcast",
    "linux",
    "Device to broadcast packets on Linux platform"
};

void registerWiFiDevice() {
    DeviceFactory::registerDevice(
        WiFiDevice::deviceInfo,
        [](const DeviceInfo& info) -> IDevice* {
            return new WiFiDevice();
        }
    );
}

}
}
