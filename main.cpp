#include <syslog.h>
#include <unistd.h>

#include "piece-fw.h"

static char platformUniqueId[16];
static int argumentCount = 0;
static const char** arguments = nullptr;

namespace piece {

void generateUniqueId() {
    uint32_t pid = hash::calculate(getpid());
    B64_encode((uint8_t*)&pid, platformUniqueId, 4);
    platformUniqueId[6] = 0;
}

}

int main(int argc, const char** args) {
    openlog("PIECE", LOG_NDELAY, LOG_USER);
    arguments = args;
    argumentCount = argc;
    piece::setup();
    for(;;)
        piece::loop();
    return 0;
}
