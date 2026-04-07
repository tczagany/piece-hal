#include <syslog.h>
#include <unistd.h>

#include "piece.h"
#include "core/z85.h"

static char platformUniqueId[16];
static int argumentCount = 0;
static const char** arguments = nullptr;

namespace piece {

void generateUniqueId() {
    uint32_t pid = hash::calculate(getpid());
    B64_encode((uint8_t*)&pid, platformUniqueId, 4);
    platformUniqueId[6] = 0;
}

const char* getCliArgByName(const char* sn, const char* ln) {
    for(int i = 0; i < argumentCount; i++) {
        auto str = arguments[i];
        if ((strcmp(str, sn) == 0 || strcmp(str, ln) == 0) && argumentCount > i + 1) {
            return arguments[i + 1];
        }
    }
    return nullptr;
}

string getPlatformArg(const string & name, int index) {
    if (name == "cli-arg" && argumentCount > index)
        return string{arguments[index]};
    if (name == "unique-id" && index == 1) {
        if (platformUniqueId[0] == 0)
            generateUniqueId();
        return platformUniqueId;
    }
    if (name == "cli-inst-name" && index == 1)
        return getCliArgByName("-n", "--name");
    if (name == "cli-diag-out" && index == 1)
        return getCliArgByName("-d", "--dout");
    if (name == "cli-net-config" && index == 1)
        return getCliArgByName("-l", "--ncfg");
    if (name == "cli-opt1" && index == 1)
        return getCliArgByName("-o1", "--opt1");
    if (name == "cli-opt2" && index == 1)
        return getCliArgByName("-o2", "--opt2");
    if (name == "cli-opt3" && index == 1)
        return getCliArgByName("-o3", "--opt3");
    return string{};
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
