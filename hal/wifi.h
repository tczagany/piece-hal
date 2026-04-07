#ifndef PIECE_HAL_WIFI_H
#define PIECE_HAL_WIFI_H

#include "../core/string.h"
#include "interface.h"

namespace piece {
namespace hal {

class IWiFiDevice : public ITransmission {
public:
    using netEnumCallback = void (*)(string, string, int, int, void*);

public:
    virtual void startSSIDQuery(netEnumCallback cb, void* param) = 0;
    virtual void cancelSSIDQuery() = 0;
};

}
}

#endif //PIECE_HAL_WIFI_H
