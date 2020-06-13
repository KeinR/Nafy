#include "Speaker.h"

#include "../core/error.h"
#include "../env/env.h" // DEBUG

#include <iostream> // DEBUG
#include <chrono>
#include <cmath>

// Must be no less than 10, because it's divided by 10 to get the sleep
// time while waiting for the buffer swap... Though, due to the unpredictableness
// of sleeping, you may want to keep it in the tripple digits.
// Honestly, or sleep for like 1 nanosecond like in the main Context loop...
#define STREAM_BUFFER_MILLIS 1000

// It's stupid. It's moronic.
// The compiler will deal with it.
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
    // if the handle hasn't been moved away, stop any streams or playback
    // and delete the OpenAL source object
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
    // Notifies any streams that they should change their playback position
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
    alGetSourcei(handle, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void nafy::Speaker::playStream(AudioStream &stream) {
    stop();
    // Attempt to lock the given stream
    if (stream.lock()) {
        throw al_error("Cannot play stream as it is in use");
    }
    streamThread = std::thread(doPlayStreamBlocked, this, &stream);
}
void nafy::Speaker::playStreamBlocked(AudioStream &stream) {
    stop();
    if (stream.lock()) {
        throw al_error("Cannot play stream as it is in use");
    }
    doPlayStreamBlocked(&stream);
}

void nafy::Speaker::doPlayStreamBlocked(AudioStream *stream) {
    streaming = true;
    shouldStream = true;
    const soundInfo &info = stream->getInfo();
    // Calculate the size of the intermediary buffer, the one
    // that carries data from the stream to the OpenAL buffers
    constexpr float bufferSeconds = STREAM_BUFFER_MILLIS / 1000.0f;
    int bufferSize = std::ceil(bufferSeconds * info.bytesPerSecond);
    bufferSize -= bufferSize % info.blockAlign;

    // How many milliseconds the stream will sleep for while waiting
    // for a buffer to finish playing
    constexpr int waitMillis = STREAM_BUFFER_MILLIS / 10;

    char *buffer = new char[bufferSize];
    // I've seem people use more than two buffers, but honestly, I don't get why.
    // Two should be enough to ensure that the stream is playing continuously...
    ALuint alBuffers[2];
    alGenBuffers(2, alBuffers);

    // Set the playback offest
    stream->seek(seek);
    seekChanged.store(false);
    try {
        // There are many things that can cause the stream to stop.
        // `shouldStop` is induced by the stream itself realizing that it's reached
        // the end of the stream.
        // `shouldStream` is set somewhere else in the class (stop() method), and it
        // signals that the stream should stop itself as fast as possible.
        bool shouldStop;
        do {

            // Set up buffers' initial state - filled
            for (int i = 0; i < 2; i++) {
                streampos_t length = bufferSize;
                shouldStop = stream->read(buffer, length);
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
                // Play until there is a free buffer to re-fill
                do {
                    alGetSourcei(handle, AL_BUFFERS_PROCESSED, &processed);
                    std::this_thread::sleep_for(std::chrono::milliseconds(waitMillis));
                } while (!processed && shouldStream);
                if (!shouldStream) {
                    break;
                }
                streampos_t length = bufferSize;
                if (seekChanged) {
                    stream->seek(seek);
                    seekChanged.store(false);
                }
                // This can throw an exception, hence why we leave the sources queued...
                // According to the OpenAL docs, the designers, in their infinite wisdom,
                // decided to make it so that an "unqueue operation will only take place
                // if all n buffers can be removed from the queue" with "n" being the integer
                // after "handle". Of course, I could do alSourceUnqueueBuffers(handle, 1, &buf)
                // twice at cleanup and just clear errors, but this solution feels better.
                shouldStop = stream->read(buffer, length);
                ALuint buf;
                alSourceUnqueueBuffers(handle, 1, &buf);
                alBufferData(buf, info.format, buffer, length * sizeof(char), info.frequency);
                alSourceQueueBuffers(handle, 1, &buf);
            }
        } while (loop && shouldStream && !shouldStop);
    } catch (al_error &e) {
        // Catch any al_errors thrown and clean up before passing the tortch
        // Ug the copy paste
        stopPlaying();
        alSourceUnqueueBuffers(handle, 2, alBuffers);
        alDeleteBuffers(2, alBuffers);
        delete[] buffer;
        stream->release();
        shouldStream = false;
        streaming = false;
        throw e;
    }

    // Clean up resources and end
    alSourceUnqueueBuffers(handle, 2, alBuffers);
    alDeleteBuffers(2, alBuffers);
    delete[] buffer;
    stream->release();
    shouldStream = false;
    streaming = false;
}

void nafy::Speaker::playBlocked() {
    // It's really straightforward, more a convinience than anything
    play();
    while (isPlaying()) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(1));
    }
}

void nafy::Speaker::play() {
    alSourcePlay(handle);
}

void nafy::Speaker::stopPlaying() {
    // I thought it was done with settting an alSourcei value at first,
    // until I checked the docs again...
    alSourceStop(handle);
}

void nafy::Speaker::stop() {
    // Stop streams and stop playback
    if (isStreaming()) {
        shouldStream = false;
        streamThread.join();
    }
    stopPlaying();
}
