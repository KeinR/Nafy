#include "Speaker.h"

#include <chrono>
#include <thread>

static int alBool(bool b) {
    return b ? AL_TRUE : AL_FALSE;
}

nafy::Speaker::Speaker() {
    alGenSources(1, &handle);
}
nafy::Speaker::~Speaker() {
    close();
}

void nafy::Speaker::close() {
    if (handle) {
        alDeleteSources(1, &handle);
    }
}
void nafy::Speaker::steal(Speaker &other) {
    handle = other.handle;
    other.handle = 0;
}

nafy::Speaker::Speaker(Speaker &&other) {
    steal(other);
}
nafy::Speaker &nafy::Speaker::operator=(Speaker &&other) {
    close();
    steal(other);
    return *this;
}

void nafy::Speaker::setGain(float val) {
    alSourcef(handle, AL_GAIN, val);
}
void nafy::Speaker::setPitch(float val) {
    alSourcef(handle, AL_PITCH, val);
}

void nafy::Speaker::setPosition(float x, float y, float z) {
    alSource3f(handle, AL_POSITION, x, y, z);
}
void nafy::Speaker::setVelocity(float x, float y, float z) {
    alSource3f(handle, AL_VELOCITY, x, y, z);
}
void nafy::Speaker::setDirection(float x, float y, float z) {
    alSource3f(handle, AL_DIRECTION, x, y, z);
}

void nafy::Speaker::setRelative(bool val) {
    alSourcei(handle, AL_SOURCE_RELATIVE, alBool(val));
}
void nafy::Speaker::setLooping(bool val) {
    alSourcei(handle, AL_LOOPING, alBool(val));
}
void nafy::Speaker::setTime(float seconds) {
    alSourcef(handle, AL_SEC_OFFSET, seconds);
}
void nafy::Speaker::setBuffer(const SoundBuffer &buffer) {
    alSourcei(handle, AL_BUFFER, buffer.get());
}
// void nafy::Speaker::queueBuffer(const SoundBuffer &buffer) {

// }

bool nafy::Speaker::isPlaying() {
    ALenum state;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void nafy::Speaker::playBlocked() {
    play();
    while (isPlaying()) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

void nafy::Speaker::play() {
    alSourcePlay(handle);
}

void nafy::Speaker::stop() {
    alSourcei(handle, AL_SOURCE_STATE, AL_STOPPED);
}
