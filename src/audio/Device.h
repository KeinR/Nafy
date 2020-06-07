#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include "oal.h"

#include "AudioContext.h"

#include <cstddef>

// Device - the actual (usually physical) speaker... I think...

namespace nafy {
    class Device {
        ALCdevice *handle;
        
        void close();
        void steal(Device &other);
        // I have no idea why you would WANT to copy, but
        // I'll just assume the best
        void copy(const Device &other);
    public:
        // NULL selects the default, "preferred" device
        Device(const ALCchar *devicename = NULL);
        Device(Device &&other);
        Device(const Device &other);
        ~Device();
        Device &operator=(Device &&other);
        Device &operator=(const Device &other);

        AudioContext makeContext();
    };
};

#endif
