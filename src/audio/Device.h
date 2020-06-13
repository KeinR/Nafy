#ifndef DEVICE_H_INCLUDED
#define DEVICE_H_INCLUDED

#include "oal.h"

#include <cstddef>

/*
* Device - the actual (usually physical) speaker... I think...
* Encapsulates all OpenAL objects - for our purposes, those are buffers, contexts and speakers.
* However it only directly wraps Contexts (AudioContext) and buffers (SoundBuffer)
* Needs a context to do anything.
* Cannot be bound itself, needs one of its AudioContexts to bind for it to be set current. 
*/

namespace nafy {
    class Device {
        ALCdevice *handle;

        void close();
        void steal(Device &other);
        // I have no idea why you would WANT to copy, but
        // I'll just assume you have good reasons
        void copy(const Device &other);
    public:
        // NULL selects the default, "preferred" device
        Device(const ALCchar *devicename = NULL);
        Device(Device &&other);
        Device(const Device &other);
        ~Device();
        Device &operator=(Device &&other);
        Device &operator=(const Device &other);

        /* private, but I mean do what you want */

        ALCdevice *get() const;
    };
};

#endif
