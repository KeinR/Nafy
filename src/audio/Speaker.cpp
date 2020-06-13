#include "Speaker.h"

#include "../core/error.h"
#include "../env/env.h" // DEBUG

#include <iostream> // DEBUG
#include <chrono>
#include <cmath>

// Must be no less than 10
#define STREAM_BUFFER_MILLIS 1000

static int alBool(bool b) {
    return b ? AL_TRUE : AL_FALSE;
}

nafy::Speaker::Speaker():
    streaming(false), shouldStream(false), loop(false), seekChanged(false), seek(0.0f)
{
    alGenSources(1, &handle);
}
nafy::Speaker::~Speaker() {
    close();
}

void nafy::Speaker::close() {
    if (handle) {
        stop();
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
    loop = val;
    alSourcei(handle, AL_LOOPING, alBool(val));
}
void nafy::Speaker::setTime(float seconds) {
    alSourcef(handle, AL_SEC_OFFSET, seconds);
    seek.store(seconds);
    seekChanged.store(true);
}
void nafy::Speaker::setBuffer(const SoundBuffer &buffer) {
    alSourcei(handle, AL_BUFFER, buffer.get());
}

bool nafy::Speaker::isStreaming() {
    return streaming;
}

bool nafy::Speaker::isPlaying() {
    ALenum state;
    std::cout << "ERROR pre-ffplay" << getALErrorStr(alGetError()) << std::endl;
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    std::cout << "ERROR ffplay" << getALErrorStr(alGetError()) << std::endl;
    return state == AL_PLAYING;
}

void nafy::Speaker::playStream(AudioStream &stream) {
    std::cout << "playStream" << std::endl;
    stop();
    if (stream.lock()) {
        throw al_error("Cannot play stream as it is in use");
    }
    streamThread = std::thread(doPlayStreamBlocked, this, std::ref(stream));
}
void nafy::Speaker::playStreamBlocked(AudioStream &stream) {
    std::cout << "playStreamBlocked" << std::endl;
    stop();
    if (stream.lock()) {
        throw al_error("Cannot play stream as it is in use");
    }
    doPlayStreamBlocked(stream);
}

// TEMP
bool alErrorYes() {
    return alGetError() != AL_NO_ERROR;
}

void nafy::Speaker::doPlayStreamBlocked(AudioStream &stream) {
    streaming = true;
    shouldStream = true;
    const soundInfo &info = stream.getInfo();
    constexpr float bufferSeconds = STREAM_BUFFER_MILLIS / 1000.0f;
    int bufferSize = std::ceil(bufferSeconds * info.bytesPerSecond);
    bufferSize -= bufferSize % info.blockAlign;

    constexpr int waitMillis = STREAM_BUFFER_MILLIS / 10;


    char *buffer = new char[bufferSize];
    ALuint alBuffers[2];
    alGenBuffers(2, alBuffers);

    stream.seek(seek);
    seekChanged.store(false);
    try {
        bool shouldStop; // Should stop is more... graceful than !shouldStream
        do {

            for (int i = 0; i < 2; i++) {
                streampos_t length = bufferSize;
                shouldStop = stream.read(buffer, length);
                alBufferData(alBuffers[i], info.format, buffer, length * sizeof(char), info.frequency);
            }

            alSourceQueueBuffers(handle, 2, alBuffers);
            play();

            for (;;) {
                int processed = 0;
                if (shouldStop) {
                    // Play the rest of the queue
                    do {
                        alGetSourcei(handle, AL_BUFFERS_PROCESSED, &processed);
                        std::this_thread::sleep_for(std::chrono::milliseconds(waitMillis));
                    } while (processed < 2 && shouldStream);
                    break;
                }
                do {
                    alGetSourcei(handle, AL_BUFFERS_PROCESSED, &processed);
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitMillis));
                } while (!processed && shouldStream);
                if (!shouldStream) {
                    break;
                }
                streampos_t length = bufferSize;
                if (seekChanged) {
                    stream.seek(seek);
                    seekChanged.store(false);
                }
                shouldStop = stream.read(buffer, length);
                ALuint buf;
                alSourceUnqueueBuffers(handle, 1, &buf);
                alBufferData(buf, info.format, buffer, length * sizeof(char), info.frequency);
                alSourceQueueBuffers(handle, 1, &buf);
            }
        } while (loop && shouldStream && !shouldStop);
    } catch (al_error &e) {
        stopPlaying();
        alSourceUnqueueBuffers(handle, 2, alBuffers);
        alDeleteBuffers(2, alBuffers);
        delete[] buffer;
        stream.release();
        shouldStream = false;
        streaming = false;
        throw e;
    }

    int processedd;
    alGetSourcei(handle, AL_BUFFERS_PROCESSED, &processedd);
    alSourceUnqueueBuffers(handle, 2, alBuffers);
    alDeleteBuffers(2, alBuffers);
    delete[] buffer;
    stream.release();
    shouldStream = false;
    streaming = false;
    std::cout << "ERROR -> " << getALErrorStr(alGetError()) << std::endl;
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

void nafy::Speaker::stopPlaying() {
    alSourceStop(handle);
}

void nafy::Speaker::stop() {
    std::cout << "STOP CALLED" << std::endl;
    if (isStreaming()) {
        shouldStream = false;
        streamThread.join();
    }
    stopPlaying();
}
