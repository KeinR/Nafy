#include "SoundBuffer.h"

#include <iostream> // DEUBG

nafy::SoundBuffer::SoundBuffer(const SoundData &data):
    SoundBuffer(data.data.get(), data.dataBytes, data.frequency, data.format) {
}
nafy::SoundBuffer::SoundBuffer(const void *data, ALsizei dataBytes, ALsizei frequency, ALenum format) {
    alGenBuffers(1, &buffer);
    std::cout << "DATA = " << (data == NULL) << std::endl;
    std::cout << "DATA = " << (std::size_t)data << std::endl;
    alGetError();
	alBufferData(buffer, format, data, dataBytes, frequency);
    ALenum err = alGetError();
    if (err != AL_NO_ERROR) {
        std::cout << "Fuck " << err << std::endl;
        switch (err) {
            case AL_INVALID_NAME: std::cout << "Bad name" << std::endl; break;
            case AL_INVALID_OPERATION: std::cout << "Bad operation" << std::endl; break;
            case ALC_INVALID_CONTEXT: std::cout << "ALC_INVALID_CONTEXT"; break;
            case AL_INVALID_VALUE: std::cout << "AL_INVALID_VALUE"; break;
            case AL_OUT_OF_MEMORY: std::cout << "AL_OUT_OF_MEMORY"; break;
            default: std::cout << "Dunno'" << std::endl;
        }
        std::cout << std::endl;
    }
}
nafy::SoundBuffer::~SoundBuffer() {
    close();
}
void nafy::SoundBuffer::close() {
    if (buffer) {
        alDeleteBuffers(1, &buffer);
    }
}
void nafy::SoundBuffer::steal(SoundBuffer &other) {
    buffer = other.buffer;
    other.buffer = 0;
}

nafy::SoundBuffer::SoundBuffer(SoundBuffer &&other) {
    steal(other);
}
nafy::SoundBuffer &nafy::SoundBuffer::operator=(SoundBuffer &&other) {
    close();
    steal(other);
    return *this;
}

ALuint nafy::SoundBuffer::get() const {
    return buffer;
}
