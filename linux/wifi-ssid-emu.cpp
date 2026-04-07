#include "piece.h"

#include "hal/wifi.h"
#include "hal/system.h"
#include "hal/interface.h"

namespace piece {
namespace hal {

class WiFiSSIDDeviceEmulator : public IWiFiDevice {
friend void registerSSIDRetrieverDevice();
public:
    WiFiSSIDDeviceEmulator()
        : callbackParam(nullptr), wifiStats{0,0,0,0},
          nextIdTime(0), delay(200), count(5)
    {
    }

    ~WiFiSSIDDeviceEmulator() {
    }

    int initiate(variant settings) override {
        if (!settings.find("count").empty())
            count = settings["count"]->as_int();
        if (!settings.find("delay").empty())
            delay = settings["delay"]->as_int();
        return 0;
    }

    const DeviceInfo& info() override {
        return deviceInfo;
    }

    void destroy() override {
        cancelSSIDQuery();
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
        callback = cb;
        callbackParam = param;
        NativeScheduler::inst().activate("hal-ssid", this, &WiFiSSIDDeviceEmulator::process, 1, true);
        nextIdIndex = 0;
        nextIdTime = hal::System::uptime_ms() + delay;
    }

    void cancelSSIDQuery() override {
        if (nextIdTime != 0) {
            nextIdTime = 0;
            nextIdIndex = 0;
            NativeScheduler::inst().deactivate("hal-ssid");
        }
    }

protected:
    netEnumCallback callback;
    void* callbackParam;
    statistics wifiStats;
    uint32_t nextIdTime;
    uint32_t nextIdIndex;
    uint32_t count;
    uint32_t delay;
    static DeviceInfo deviceInfo;

    bool process() {
        auto now = hal::System::uptime_ms();
        if (nextIdIndex == count) {
            cancelSSIDQuery();
            callback(string{}, string{}, 0, 0, callbackParam);
        }
        else if (nextIdTime <= now) {
            ostringstream oss(32);
            oss << "ssid_" << nextIdIndex << "_" << now;
            string security("WPA2");
            callback(oss.str(), security, hal::System::random(0, 255),
                hal::System::random(30, 90), callbackParam);
            nextIdTime = now + delay;
            nextIdIndex++;
        }
        return true;
    }
};

DeviceInfo WiFiSSIDDeviceEmulator::deviceInfo = {
    nullptr, nullptr,
    DeviceType::wifi,
    DeviceClass::transmission,
    "wifi-ssid-emu",
    "piece",
    "Emulator device to retrieve the available WiFi ssids"
};

void registerSSIDRetrieverDevice() {
    DeviceFactory::registerDevice(
        WiFiSSIDDeviceEmulator::deviceInfo,
        [](const DeviceInfo& info) -> IDevice* {
            return new WiFiSSIDDeviceEmulator();
        }
    );
}

}
}
