#ifndef AUDIOCONTEXT_H_INCLUDED
#define AUDIOCONTEXT_H_INCLUDED

#include "oal.h"

// NOTE THAT NOTHING WILL WORK IF YOU HAVEN'T BOUND THE CONTEXT!!!

namespace nafy {
    class AudioContext {
        ALCcontext *handle;

        void close();
        void steal(AudioContext &other);
        void copy(const AudioContext &other);
    public:
        AudioContext(ALCcontext *handle);
        AudioContext(AudioContext &&other);
        AudioContext(const AudioContext &other);
        ~AudioContext();
        AudioContext &operator=(AudioContext &&other);
        AudioContext &operator=(const AudioContext &other);

        // Returns false on failure
        bool bind();
    };
};

#endif
