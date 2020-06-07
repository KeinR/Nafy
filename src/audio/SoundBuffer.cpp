#include "SoundBuffer.h"

nafy::SoundBuffer::SoundBuffer(const SoundData &data):
    SoundBuffer(data.data.get(), data.dataBytes, data.frequency, data.format) {
}
nafy::SoundBuffer::SoundBuffer(const void *data, ALsizei dataBytes, ALsizei frequency, ALenum format) {
    alGenBuffers(1, &buffer);
	alBufferData(buffer, format, data, dataBytes, frequency);
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
