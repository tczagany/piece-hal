#include "diagstream.h"

namespace piece {
namespace hal_template {

hal::DeviceInfo DiagStream::deviceInfo = {
    hal::DeviceType::log_port,
    hal::DeviceClass::unknown,
    "log",
    "undefined",
    "Log stream template"
};

DeviceListEntry DiagStream::deviceEntry = {
    nullptr, nullptr, nullptr
};

int DiagStream::initiate(variant_t settings) {
    return 0;
}

const hal::DeviceInfo& DiagStream::info() {
    return deviceInfo;
}

void DiagStream::destroy() {
}

bool DiagStream::canWrite() {
    return false;
}

void DiagStream::flush() {
}

void DiagStream::write(char) {
}

void DiagStream::write(const char*) {
}

void DiagStream::write(const uint8_t*, int) {
}

void DiagStream::hwIndicator(int, int) {
}

}
}
