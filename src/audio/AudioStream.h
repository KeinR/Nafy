#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED

#include <fstream>
#include <stb/stb_vorbis.h>
#include "oal.h"

/*
* Provices support for streaming in Speaker.
* Streaming, if you didn't know, is basically required if you
* don't want to have 60MB of decoded PCM data hanging around
* in memory.
* For (rather) small sounds, (ex. sound effects) just load them into memory using SoundData.
* Cuz like, streaming uses a buffer of like 2-3 seconds, so if the sound is short enough, it's just not worth it.
* Noteworthy is how these are device and context indepent, because of how they're streaming raw PCM data and
* aren't actually creating any buffers.
* Only two formats are supported: PCM WAV and ogg Vorbis
*/


namespace nafy {
    // Metadata passed to a speaker so that it can properly
    // create buffers
    struct soundInfo {
        int bytesPerSecond;
        short blockAlign;
        ALenum format;
        ALsizei frequency;
    };

    typedef long int streampos_t;

    class AudioStream {
        bool locked;
    public:
        AudioStream();
        virtual ~AudioStream();
        virtual const soundInfo &getInfo() = 0;
        virtual bool read(char *buffer, streampos_t &length) = 0; // Return true if at end
        virtual void seek(float seconds) = 0;
        bool inUse();
        // So, if it's locked already, notify the caller.
        // Otherwise, secure it and tell the caller that they're
        // all good to use the object.
        // so...
        // Marks the stream as in-use.
        // Returns true if the stream was already in use, ie, the value of `inUse`
        // The purpose of this is rooted in how streams can happen concurrently.
        // The lock (should) always happen on one thread, the main thread.
        // The release happens on the spawned threads.
        // See Speaker for more details.
        bool lock();
        // Releases lock
        void release();
    };

    // Load a PCM WAV file
    class WavStream: public AudioStream {
        std::ifstream stream;
        streampos_t start;
        streampos_t end;
        soundInfo info;
    public:
        WavStream(const std::string &file);
        const soundInfo &getInfo() override;
        bool read(char *buffer, streampos_t &length) override;
        void seek(float seconds) override;
    };

    // Load a vorbis (.ogg) file using stb_vorbis
    class VorbisStream: public AudioStream {
        // Handle to stb_vorbis decoder
        stb_vorbis *decoder;
        // Metadata from decoder
        stb_vorbis_info vInfo;
        soundInfo info;
    public:
        VorbisStream(const std::string &file);
        const soundInfo &getInfo() override;
        bool read(char *buffer, streampos_t &length) override;
        void seek(float seconds) override;
    };

}

#endif
