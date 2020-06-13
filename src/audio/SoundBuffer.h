#ifndef SOUNDBUFFER_H_INCLUDED
#define SOUNDBUFFER_H_INCLUDED

#include "oal.h"

#include "SoundData.h"

/*
* SoundBuffers are specific to a given device.
* They are independent of contexts.
* https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf#page=8
* Note: OpenAL buffers copy the data given to them, so you can, and should, free the data afterwards.
*/

namespace nafy {
    class SoundBuffer {
        ALuint buffer;
        void close();
        void steal(SoundBuffer &other);
    public:
        // From a SoundData object
        SoundBuffer(const SoundData &data);
        // From meta and actual data
        SoundBuffer(const void *data, ALsizei dataBytes, ALsizei frequency, ALenum format);
        // From what the docs have said, it appears to be impossible to read data from a OpenAL
        // sound buffer. You can get a pointer to the source data that was given to it, but
        // there's not garuntee that it's not been free'd/delete'd. Ergo, copying is prevented.
        SoundBuffer(SoundBuffer &&other);
        ~SoundBuffer();
        SoundBuffer &operator=(SoundBuffer &&other);

        // Gets the wrapped OpenAL buffer
        ALuint get() const;
    };
};

#endif
