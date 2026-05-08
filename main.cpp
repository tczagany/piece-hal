#include "diagstream.h"
#include "display.h"
#include "gpio.h"
#include "serial.h"
#include "ssidenum.h"
#include "system.h"
#include "transmission.h"

int main(int argc, const char** args) {
    piece::DeviceFactory::registerDevice(
        piece::hal_template::DiagStream::deviceEntry,
        piece::hal_template::DiagStream::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::DiagStream();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::Display::deviceEntry,
        piece::hal_template::Display::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::Display();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::GPIO::deviceEntry,
        piece::hal_template::GPIO::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::GPIO();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::Serial::deviceEntry,
        piece::hal_template::Serial::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::Serial();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::SerialEnumerator::deviceEntry,
        piece::hal_template::SerialEnumerator::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::SerialEnumerator();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::SSIDEnumerator::deviceEntry,
        piece::hal_template::SSIDEnumerator::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::SSIDEnumerator();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::System::deviceEntry,
        piece::hal_template::System::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::System();
        }
    );
    piece::DeviceFactory::registerDevice(
        piece::hal_template::Transmission::deviceEntry,
        piece::hal_template::Transmission::deviceInfo,
        []() -> piece::hal::IDevice* {
            return new piece::hal_template::Transmission();
        }
    );

    piece::setup();
    for(;;)
        piece::loop();
    return 0;
}
