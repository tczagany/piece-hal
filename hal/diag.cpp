#include "piece.h"
#include "diag.h"

namespace piece {
namespace hal {

char Diag::frwClassName[32];
char Diag::frwInstanceName[32];
uint8_t Diag::contentFlags = 0;
Diag::lineIOCallback Diag::ioCallback = nullptr;

void Diag::set(const char* className, const char* instName) {
    strncpy(frwClassName, className, 32);
    strncpy(frwInstanceName, instName, 32);
}

void Diag::set(lineIOCallback cb) {
    ioCallback = cb;
}

void Diag::set(uint8_t flags) {
    contentFlags = flags;
}

void Diag::write(const uint8_t* buffer, int size) {
    IOStream io;
    hexDump(buffer, size, &io);
    io.flush();
}

void Diag::logInternal(str_ostream& oss) {
    oss << "\n";
}

}
}
