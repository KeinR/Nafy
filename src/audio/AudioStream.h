#ifndef AUDIOSTREAM_H_INCLUDED
#define AUDIOSTREAM_H_INCLUDED

#include <fstream>
#include <stb/stb_vorbis.h>
#include "oal.h"

// Note that you cannot be using more than one stream at a time

namespace nafy {
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
        // Marks the stream as in-use.
        // Returns true if the stream was already in use, ie, the value of `inUse`
        bool lock();
        // Releases lock
        void release();
    };

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

    class VorbisStream: public AudioStream {
        stb_vorbis *decoder;
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
