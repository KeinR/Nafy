#ifndef SOUNDBUFFER_H_INCLUDED
#define SOUNDBUFFER_H_INCLUDED

#include "oal.h"

#include "SoundData.h"

// SoundBuffers are specific to a given device

namespace nafy {
    class SoundBuffer {
        ALuint buffer;
        void close();
        void steal(SoundBuffer &other);
    public:
        SoundBuffer(const SoundData &data);
        SoundBuffer(SoundBuffer &&other);
        ~SoundBuffer();
        SoundBuffer &operator=(SoundBuffer &&other);

        ALuint get() const;
    };
};

#endif
