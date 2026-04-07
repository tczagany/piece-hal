#include "system.h"
#include "core/strstream.h"

namespace piece {
namespace hal {

template<typename... T>
void Diag::log(const T& ...args) {
    IOStream io;
    str_ostream oss(io);
    bool needSeparator = false;
    if (Diag::contentFlags & Diag::cnt_time) {
        oss << System::uptime_ms() << "ms";
        needSeparator = true;
    }
    if (Diag::contentFlags & Diag::cnt_nclass) {
        if (needSeparator)
            oss << " ";
        oss << frwClassName;
    }
    if (Diag::contentFlags & Diag::cnt_ninst) {
        if (Diag::contentFlags & Diag::cnt_nclass)
            oss << "/";
        else if (needSeparator)
            oss << " ";
        oss << frwInstanceName;
    }
    if (Diag::contentFlags & Diag::cnt_message) {
        if (needSeparator)
            oss << " ";
        logInternal(oss, args...);
    }
    io.flush();
}

template<typename A, typename... T>
void Diag::logInternal(str_ostream& oss, const A& arg, const T& ...args) {
    oss << arg;
    logInternal(oss, args...);
}

}
}
