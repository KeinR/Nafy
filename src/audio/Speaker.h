#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED

#include "oal.h"

#include "SoundBuffer.h"

// Speakers are specific to a given context.
// OpenAL calls them "sources", which I guess is more accurate

// TODO: Copy constructors, or at least a derive() function

namespace nafy {
    class Speaker {
        ALuint handle;
        void close();
        void steal(Speaker &other);
    public:
        Speaker();
        Speaker(Speaker &&other);
        ~Speaker();
        Speaker &operator=(Speaker &&other);

        void setGain(float val);
        void setPitch(float val);

        void setPosition(float x, float y, float z);
        void setVelocity(float x, float y, float z);
        void setDirection(float x, float y, float z);
        
        // Is the xyz position relative to the listener?
        // Default is false
        void setRelative(bool val);
        // Loop audio playback?
        void setLooping(bool val);
        // Second offset of audio
        void setTime(float seconds);
        // Set sound buffer for playback
        void setBuffer(const SoundBuffer &buffer);
        // void queueBuffer(const SoundBuffer &buffer);


        bool isPlaying();

        // Block this thread until done
        void playBlocked();
        // Play async
        void play();
        // Stop playing.
        // No averse effect if not playing.
        void stop();
    };
};

#endif
