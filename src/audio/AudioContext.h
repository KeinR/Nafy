#ifndef AUDIOCONTEXT_H_INCLUDED
#define AUDIOCONTEXT_H_INCLUDED

#include "oal.h"

#include "Device.h"

// NOTE THAT NOTHING WILL WORK IF YOU HAVEN'T BOUND THE CONTEXT!!!

/*
* The context, encapsulates the speakers and listener.
* See the diagram here -> https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf#page=8
* Note well that speakers will NOT work if there is no bound context.
* Would it be a good idea to auto-bind a context on creation to account for mistakes such as those?
* NO!
* That has the potentual to mess things up!
* Also worthy of note: know how a buffer is specific to a device? Well, you can't bind a device. Hence,
* binding a context also binds its device. Therefore, you really can't do ANYTHING until you have bound
* a context (except of course, making the device and context).
*/


namespace nafy {
    class AudioContext {
        ALCcontext *handle;

        void close();
        void steal(AudioContext &other);
        // Won't copy over bindings, etc. of course
        void copy(const AudioContext &other);
    public:
        AudioContext(const Device &parent);
        AudioContext(AudioContext &&other);
        AudioContext(const AudioContext &other); // see copy()
        ~AudioContext();
        AudioContext &operator=(AudioContext &&other);
        AudioContext &operator=(const AudioContext &other); // see copy()

        // Binds the context, causing all speakers etc. to be created inside this context.
        // See https://www.openal.org/documentation/OpenAL_Programmers_Guide.pdf#page=8
        // Returns false on failure.
        bool bind() const;
    };
};

#endif
