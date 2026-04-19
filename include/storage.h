#ifndef PIECE_HAL_STORAGE_H
#define PIECE_HAL_STORAGE_H

#include "device.h"

namespace piece {
namespace hal {

struct IStorage : public IDevice {
    virtual bool mount() = 0;
    virtual void unmount() = 0;
    virtual int free() = 0;
    virtual int size() = 0;
    virtual bool test(const char* path) = 0;
    virtual bool remove(const char* path) = 0;
    virtual int fopen(const char* path) = 0;
    virtual int fcreate(const char* path) = 0;
    virtual void fclose(int file) = 0;
    virtual void fsize(int file) = 0;
    virtual int fread(int file, char* buffer, int size, int offset = 0) = 0;
    virtual int fwrite(int file, char* buffer, int size, int offset = 0) = 0;
    virtual void fflush(int file) = 0;
};

}
}

#endif //PIECE_HAL_STORAGE_H
