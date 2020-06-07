#include "Device.h"

nafy::Device::Device(const ALCchar *devicename): handle(alcOpenDevice(devicename)) {
}

nafy::Device::~Device() {
    close();
}

void nafy::Device::close() {
    if (handle != nullptr) {
        alcCloseDevice(handle);
    }
}

void nafy::Device::steal(Device &other) {
    handle = other.handle;
    other.handle = nullptr;
}

void nafy::Device::copy(const Device &other) {
    handle = alcOpenDevice(alcGetString(other.handle, ALC_DEVICE_SPECIFIER));
}

nafy::Device::Device(Device &&other) {
    steal(other);
}
nafy::Device::Device(const Device &other) {
    copy(other);
}
nafy::Device &nafy::Device::operator=(Device &&other) {
    close();
    steal(other);
    return *this;
}
nafy::Device &nafy::Device::operator=(const Device &other) {
    close();
    copy(other);
    return *this;
}

nafy::AudioContext nafy::Device::makeContext() {
    return AudioContext(alcCreateContext(handle, NULL));
}
