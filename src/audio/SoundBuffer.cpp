#include "SoundBuffer.h"

nafy::SoundBuffer::SoundBuffer(const SoundData &data) {
    alGenBuffers(1, &buffer);
	alBufferData(buffer, data.format, data.data.get(), data.dataBytes, data.frequency);
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
