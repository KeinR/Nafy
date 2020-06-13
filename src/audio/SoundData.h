#ifndef SOUNDDATA_H_INCLUDED
#define SOUNDDATA_H_INCLUDED

#include <memory>

#include "oal.h"

/*
* Here is where you load small sound files into memory in their entirety.
* A load* file returns a `SoundData` object that has a shared_ptr with a custom
* deleter, along with its metadata. 
* That SoundData is used then to construct a SoundBuffer.
* Since you're often only going to have one SoundBuffer per sound,
* I recommend that you call the function in the constructor like SoundBuffer(loadWavFile("faf.wav"))
* so that it destructs automatically, or alternatively calling reset() on the `data` field in order to delete
* the PCM data.
*/

// SoundBuffers are specific to a given device, hence the use of a raw data struct

namespace nafy {
    struct SoundData {
        typedef const void data_t;
        // Uses custom deleter based on original type to avoid undefined behavior
        std::shared_ptr<data_t> data;
        // Total size in bytes
        ALsizei dataBytes;
        // Also known as sample rate
        ALsizei frequency;
        // The OpenAL format... There are 4
        ALenum format;
        SoundData(std::shared_ptr<data_t> data, ALsizei dataBytes, ALsizei frequency, ALenum format);
    };

    // Gets an OpenAL format based on the number of channels and the sample size
    // Anything other than 16 or 8 for the `bitsPerSample` will return 0
    ALenum getALFormat(int channels, int bitsPerSample);

    // Load PCM wav from memory/file. This will simply parse the header and
    // extract the raw PCM data
    SoundData loadWavFile(const std::string &path);
    SoundData loadWavMemory(const unsigned char *data, int length);
    // Load vorbis (.ogg) from memory/file. This will use the stb_vorbis
    // decoder, and will take some time due to the nature of decoding.
    // Therefore, make sure to do this during the loading phase.
    SoundData loadVorbisFile(const std::string &path);
    SoundData loadVorbisMemory(const unsigned char *data, int length);
};

#endif
