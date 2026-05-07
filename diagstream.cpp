#include "diagstream.h"

namespace piece {
namespace hal_template {

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
