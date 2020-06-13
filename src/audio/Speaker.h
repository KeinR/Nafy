#ifndef SPEAKER_H_INCLUDED
#define SPEAKER_H_INCLUDED

#include <thread>
#include <atomic>

#include "oal.h"

#include "SoundBuffer.h"
#include "AudioStream.h"

// Speakers are specific to a given context.
// OpenAL calls them "sources", which I guess is more accurate

// TODO: Copy constructors, or at least a derive() function

// Streaming not really thread safe, except where it has to be

namespace nafy {
    class Speaker {
        // OpenAL handle to source object
        ALuint handle;
        // Is the stream running?
        bool streaming;
        // Should the stream continue as it's doing?
        bool shouldStream;
        // The thread that the stream resides on, if any
        std::thread streamThread;
        // Should a stream loop?
        bool loop;
        // Tells the stream that the seek has changed,
        // and that it should change its position
        std::atomic<bool> seekChanged;
        // The current seek offest, in seconds
        std::atomic<float> seek;
        // Stop the buffer from playing
        void stopPlaying();
        // Stream the audio stream on this thread
        void doPlayStreamBlocked(AudioStream *stream);
        // Close the handle and clean up resources
        void close();
        // Move the resources from `other` to self
        void steal(Speaker &other);
    public:
        Speaker();
        Speaker(Speaker &&other);
        ~Speaker();
        Speaker &operator=(Speaker &&other);

        // Google it
        void setGain(float val);
        void setPitch(float val);

        // Position: vector for where the sound is
        void setPosition(float x, float y, float z);
        // The speed of the sound idk
        void setVelocity(float x, float y, float z);
        // I'm just implementing functions I don't know what "direction" actually does
        void setDirection(float x, float y, float z);

        // Is the xyz position relative to the listener?
        // Default is false
        void setRelative(bool val);
        // Loop audio playback?
        void setLooping(bool val);
        // Second offset of audio
        void setTime(float seconds);
        // Set sound buffer for playback
        // Do note that the SoundBuffer is monopolized when playing (I'm pretty sure)
        // Calls stop()
        void setBuffer(const SoundBuffer &buffer);
        // Calls stop() and sets buffer to NULL, allowing to be used for something else.
        void clearBuffer();

        // Streaming?
        bool isStreaming();
        // Playing audio? (includes streaming)
        bool isPlaying();

        // Streams audio from a stream.
        // Very friendly to memory
        // Do note that the AudioStream is monopolized
        void playStream(AudioStream &stream);
        // Play stream, but blocked on the same thread
        void playStreamBlocked(AudioStream &stream);
        // Block this thread until done
        void playBlocked();
        // Play async
        void play();
        // Stop literally everything - playing/streaming.
        // No averse effect if not playing.
        void stop();
    };
};

#endif
