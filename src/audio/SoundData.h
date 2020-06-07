#ifndef SOUNDDATA_H_INCLUDED
#define SOUNDDATA_H_INCLUDED

#include <memory>

#include "oal.h"

// SoundBuffers are specific to a given device, hence the use of a raw data struct

namespace nafy {
    struct SoundData {
        typedef const void data_t;
        // Uses custom deleter based on original type
        // to avoid undefined behavior
        std::shared_ptr<const void> data;
        // Total size in bytes
        ALsizei dataBytes;
        // Also known as sample rate
        ALsizei frequency;
        // The OpenAL format... There are 4
        ALenum format;
        SoundData(std::shared_ptr<const void> data, ALsizei dataBytes, ALsizei frequency, ALenum format);
    };

    // Anything other than 16 or 8 for the `bitsPerSample` will return 0
    ALenum getALFormat(int channels, int bitsPerSample);

    SoundData loadWavFile(const std::string &path);
    SoundData loadWavMemory(const unsigned char *data, int length);
    SoundData loadOggVorbisFile(const std::string &path);
    SoundData loadOggVorbisMemory(const unsigned char *data, int length);
};

#endif
