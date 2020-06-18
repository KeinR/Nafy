#include "AudioContext.h"

#include <iostream> // DEBUG
#include <cstddef>

nafy::AudioContext::AudioContext(const Device &device):
    // Create context without an attribute list in the given device...
    // Dont' know how to make one though, the docs are kinda' vague...
    handle(alcCreateContext(device.get(), NULL))
{}

nafy::AudioContext::~AudioContext() {
    close();
}

void nafy::AudioContext::close() {
    if (handle != nullptr) {
        // Set context to null if this one is in use...
        // It'll make debugging easier, and prevent seg faults
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
    // Kinda' a hack, but worth it I think to get copy semantics
    // Won't copy over bindings, etc. of course
    handle = alcCreateContext(alcGetContextsDevice(other.handle), NULL);
}

nafy::AudioContext::AudioContext(AudioContext &&other) {
    steal(other);
}
nafy::AudioContext::AudioContext(const AudioContext &other) {
    copy(other);
}
nafy::AudioContext &nafy::AudioContext::operator=(AudioContext &&other) {
    // Close because it's already been constructed with a device
    close();
    steal(other);
    return *this;
}
nafy::AudioContext &nafy::AudioContext::operator=(const AudioContext &other) {
    close();
    copy(other);
    return *this;
}

bool nafy::AudioContext::bind() const {
    return alcMakeContextCurrent(handle);
}
