#include "AudioContext.h"

#include <iostream> // DEBUG
#include <cstddef>

nafy::AudioContext::AudioContext(const Device &device): handle(alcCreateContext(device.get(), NULL)) {
}

nafy::AudioContext::~AudioContext() {
    std::cout << "destruct AudioContext" << std::endl;
    close();
}

void nafy::AudioContext::close() {
    if (handle != nullptr) {
        if (alcGetCurrentContext() == handle) {
            alcMakeContextCurrent(NULL);
        }
        alcDestroyContext(handle);
    }
}

void nafy::AudioContext::steal(AudioContext &other) {
    handle = other.handle;
    other.handle = nullptr;
}
void nafy::AudioContext::copy(const AudioContext &other) {
    handle = alcCreateContext(alcGetContextsDevice(other.handle), NULL);
}

nafy::AudioContext::AudioContext(AudioContext &&other) {
    steal(other);
}
nafy::AudioContext::AudioContext(const AudioContext &other) {
    copy(other);
}
nafy::AudioContext &nafy::AudioContext::operator=(AudioContext &&other) {
    close();
    steal(other);
    return *this;
}
nafy::AudioContext &nafy::AudioContext::operator=(const AudioContext &other) {
    close();
    copy(other);
    return *this;
}

bool nafy::AudioContext::bind() {
    return alcMakeContextCurrent(handle);
}
