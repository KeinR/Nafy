#include "AudioStream.h"


#include <iostream> // DEBUG
#include <cmath>

#include "../core/error.h"
#include "SoundData.h"

template<typename T>
static T readNumber(std::ifstream &file) {
    constexpr int end = (sizeof(T) / sizeof(char) - 1) * 8;
    T result = 0;
    for (int s = 0; s <= end; s += 8) {
        if (file.eof()) {
            throw nafy::al_error("Audio file corrupted");
        }
        result |= file.get() << s;
    }
    return result;
}

auto constexpr readInt = &readNumber<int>;
auto constexpr readShort = &readNumber<short>;

static void strEq(std::ifstream &file, const char *cstr, const char *err) {
    for (int ci = 0; cstr[ci]; ci++) {
        if (file.eof()) {
            throw nafy::al_error("Audio file corrupted");
        }
        if (file.get() != cstr[ci]) {
            throw nafy::al_error(err);
        }
    }
}

nafy::AudioStream::AudioStream(): locked(false) {
}
nafy::AudioStream::~AudioStream() {
}
bool nafy::AudioStream::inUse() {
    return locked;
}
bool nafy::AudioStream::lock() {
    if (locked) {
        return true;
    }
    locked = true;
    return false;
}
void nafy::AudioStream::release() {
    locked = false;
}

nafy::WavStream::WavStream(const std::string &file): AudioStream(), stream(file) {
    if (!stream.good()) {
        throw al_error("WAV audio stream failed to open");
    }

    strEq(stream, "RIFF", "WAV Corrupted"); // Check has RIFF header

    stream.ignore(4); // Total file size. This value is useless as length is already given to us
    strEq(stream, "WAVEfmt ", "Not PCM WAV 1"); // Check has "WAVEfmt ", signifies correct format
    if (readInt(stream) != 16) {
        throw al_error("WAV Corrupted");
    }
    if (readShort(stream) != 1) { // Check if PCM
        throw al_error("Not PCM WAV");
    }
    const int channels = readShort(stream);
    info.frequency = readInt(stream); // aka sample rate
    info.bytesPerSecond = readInt(stream); // How many bytes are read per second
    info.blockAlign = readShort(stream); // Min. bytes per block. Need to truncate to mod of this else OpenAL freaks
    const short bits_per_sample = readShort(stream); // aka "samples"
    for (int f = 0; f < 4;) {
        if (stream.eof()) {
            throw al_error("Wav file corrupted, header truncated");
        }
        if ("data"[f] == stream.get()) {
            f++;
        }
    }
    const streampos_t declaredLength = readInt(stream);

    if (!stream.good()) {
        throw al_error("WAV audio stream failed to read header");
    }

    start = stream.tellg();
    end = start + declaredLength;

    info.format = getALFormat(channels, bits_per_sample);
}

const nafy::soundInfo &nafy::WavStream::getInfo() {
    return info;
}

bool nafy::WavStream::read(char *buffer, streampos_t &length) {
    bool done = false;
    if (stream.tellg() + length >= end) {
        length = end - stream.tellg();
        length -= length % info.blockAlign;
        if (length <= 0) {
            length = 0;
            return true;
        }
        done = true;
    }
    stream.read(buffer, length);
    if (!stream.good()) {
        throw al_error("WAV audio stream failed read");
    }
    return done;
}
void nafy::WavStream::seek(float seconds) {
    streampos_t s = std::floor(info.bytesPerSecond * seconds);
    s -= s % info.blockAlign;
    stream.seekg(s);
}





nafy::VorbisStream::VorbisStream(const std::string &file): AudioStream() {
    int error = 0;
    decoder = stb_vorbis_open_filename(file.c_str(), &error, NULL);
    if (error) {
        throw al_error("Failed to init vorbis decoder");
    }
    constexpr int vorbisBitsPerSample = 16;
    vInfo = stb_vorbis_get_info(decoder);
    info.frequency = vInfo.sample_rate;
    info.format = getALFormat(vInfo.channels, vorbisBitsPerSample);
    info.bytesPerSecond = vInfo.sample_rate * vorbisBitsPerSample * vInfo.channels / 8;
    info.blockAlign = vorbisBitsPerSample * vInfo.channels / 8;
}
const nafy::soundInfo &nafy::VorbisStream::getInfo() {
    return info;
}
bool nafy::VorbisStream::read(char *buffer, streampos_t &length) {
    length -= length % 2;
    length = stb_vorbis_get_samples_short_interleaved(decoder, vInfo.channels, reinterpret_cast<short*>(buffer), length / 2) * vInfo.channels * 2;
    return length == 0;
}
void nafy::VorbisStream::seek(float seconds) {
    streampos_t s = std::floor(vInfo.sample_rate * vInfo.channels * seconds);
    s -= s % info.blockAlign;
    stb_vorbis_seek(decoder, s);
}
