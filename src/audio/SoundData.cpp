#include "SoundData.h"

#include <cstring>
#include <fstream>
#include <iostream>

#include <stb/stb_vorbis.h>

#include "../core/error.h"

nafy::SoundData::SoundData(std::shared_ptr<const void> data, ALsizei dataBytes, ALsizei frequency, ALenum format):
    data(data), dataBytes(dataBytes), frequency(frequency), format(format) {
}

template<typename T>
static T readNumber(const unsigned char *source, int &i, int length) {
    constexpr int end = (sizeof(T) / sizeof(char) - 1) * 8;
    T result = 0;
    for (int s = 0; s <= end; s += 8) {
        if (i >= length) {
            delete[] source;
            throw nafy::al_error("Audio file corrupted");
        }
        result |= source[i++] << s;
    }
    return result;
}

auto constexpr readInt = &readNumber<int>;
auto constexpr readShort = &readNumber<short>;


static void strEq(const unsigned char *source, int &i, int length, const char *cstr, const char *err) {
    for (int ci = 0; cstr[ci]; ci++) {
        if (i >= length) {
            delete[] source;
            throw nafy::al_error("Audio file corrupted");
        }
        if (source[i++] != cstr[ci]) {
            delete[] source;
            throw nafy::al_error(err);
        }
    }
}

template<typename T>
static void freeData(const void *data) {
    delete[] (const T *)data;
}

static unsigned char *loadSoundFile(const std::string &path, int &length) {
    std::ifstream file(path);
    if (!file.good()) {
        throw nafy::al_error("Failed to open audio file");
    }
    file.seekg(0, file.end);
    length = file.tellg();
    file.seekg(0, file.beg);

    unsigned char *output = new unsigned char[length];

    file.read((char *)output, length);

    if (!file.good()) {
        delete[] output;
        length = 0;
        throw nafy::al_error("Failed to read from audio file");
    }

    file.close();

    return output;
}


static nafy::SoundData doLoadVorbis(stb_vorbis *handle) {
    if (handle == NULL) {
        throw nafy::al_error("Failed to load ogg data");
    }
    stb_vorbis_info info = stb_vorbis_get_info(handle);

    const unsigned int bufferLength = stb_vorbis_stream_length_in_samples(handle) * info.channels;
    short *data = new short[bufferLength];

	stb_vorbis_get_samples_short_interleaved(handle, info.channels, data, bufferLength);

    nafy::SoundData sd(
        std::shared_ptr<nafy::SoundData::data_t>(data, freeData<short>),
        bufferLength*sizeof(short),
        info.sample_rate,
        nafy::getALFormat(info.channels, 16)
    );

    stb_vorbis_close(handle);

    return sd;
}


ALenum nafy::getALFormat(int channels, int bitsPerSample) {
    if (bitsPerSample == 16) {
        return channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    } else if (bitsPerSample == 8) {
        return channels > 1 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
    }
    return 0;
}

nafy::SoundData nafy::loadWavFile(const std::string &path) {
    int length;
    unsigned char *data = loadSoundFile(path, length);
    SoundData sd(loadWavMemory(data, length));
    delete[] data;
    return sd;
}
nafy::SoundData nafy::loadWavMemory(const unsigned char *data, int length) {
    int i = 0;

    strEq(data, i, length, "RIFF", "WAV Corrupted"); // Check has RIFF header

    i += 4; // Total file size. This value is useless as length is already given to us
    strEq(data, i, length, "WAVEfmt ", "Not PCM WAV"); // Check has "WAVEfmt "
    if (readInt(data, i, length) != 16) { // Check format
        throw al_error("WAV Corrupted");
    }
    if (readShort(data, i, length) != 1) { // PCM check
        throw al_error("Not PCM WAV");
    }
    const int channels = readShort(data, i, length);
    const int frequency = readInt(data, i, length); // aka sample rate
    i += 6; // Useless info
    const short bits_per_sample = readShort(data, i, length); // aka "samples"
    // Seek past junk data to "data" chunk
    for (int f = 0; f < 4; i++) {
        if (i >= length) {
            throw al_error("Wav file corrupted, failed to seek to \"data\" chunk");
        }
        if ("data"[f] == data[i]) {
            f++;
        }
    }
    const int declaredLength = readInt(data, i, length);

    int dataLength = std::min(length - i, declaredLength);
    std::size_t dataLengthBytes = dataLength * sizeof(char);

    char *soundData = new char[dataLength];
    std::memcpy(soundData, data + i, dataLengthBytes);

    return SoundData(
        std::shared_ptr<SoundData::data_t>(soundData, freeData<char>),
        dataLengthBytes,
        frequency,
        getALFormat(channels, bits_per_sample)
    );
}
nafy::SoundData nafy::loadVorbisFile(const std::string &path) {
    return doLoadVorbis(stb_vorbis_open_filename(path.c_str(), NULL, NULL));
}
nafy::SoundData nafy::loadVorbisMemory(const unsigned char *data, int length) {
    return doLoadVorbis(stb_vorbis_open_memory(data, length, NULL, NULL));
}
