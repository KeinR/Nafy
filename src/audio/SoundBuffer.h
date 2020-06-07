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
        // Note: OpenAL buffers copy the data given to them
        // Feel free to... _free_ the data afterwards
        SoundBuffer(const SoundData &data);
        SoundBuffer(const void *data, ALsizei dataBytes, ALsizei frequency, ALenum format);
        SoundBuffer(SoundBuffer &&other);
        ~SoundBuffer();
        SoundBuffer &operator=(SoundBuffer &&other);

        ALuint get() const;
    };
};

#endif
