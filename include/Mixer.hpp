#pragma once
#include <pybind11/pybind11.h>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_stdinc.h>

#include "miniaudio.h"

namespace py = pybind11;

namespace mixer
{
inline const size_t DefaultStreamBufferSize = 12288;
inline const int StreamBufferThreshold = 8192;

void init();
void quit();


class MAFileDecoder;

class VirtualDevice;
class Audio;
class AudioStream;

class VirtualDevice
{
public:
    VirtualDevice();
    ~VirtualDevice();

    SDL_AudioDeviceID audioDevice;
    std::vector<Audio*> connectedAudio;
    std::vector<AudioStream*> connectedStreams;

    void play();
    void pause();
private:
    float cachedVolume;
};

class Audio
{
public:
    Audio(const std::string& filepath,VirtualDevice& device, const float volume = 1.0f);
    ~Audio();

    void start(int fadeInSeconds = 0,int fadeOutSeconds = 0);
    void stop(int fadeOutSeconds);

    float volume;

    int length();
    bool ended();

    bool load(const std::string p_filepath);

private:
    SDL_AudioStream* stream = nullptr;
    std::vector<Uint8> rawData;
    SDL_AudioSpec spec;

    VirtualDevice* connectedDevice;
};

class MAFileDecoder
{
public:
    ma_decoder decoder;
    SDL_AudioSpec spec;

    MAFileDecoder(const std::string& path);
    ~MAFileDecoder();

    size_t read(Uint8* buffer,const size_t numBytes);
    void rewind();
};

class AudioStream {
public:
    AudioStream(const std::string& filepath,VirtualDevice& device,const float volume = 1.0f);
    ~AudioStream();

    void play(const int fadeInSeconds,const int fadeOutSeconds,const bool reFadeIn);
    void pause(const int fadeOutSeconds);
    void rewind();

    float volume;
    bool playing;

    int length();
    bool ended();

    void __update__();

private:
    SDL_AudioStream* stream = nullptr;
    MAFileDecoder audioDecoder;
    SDL_AudioSpec spec;
    std::vector<Uint8> grabbedData;

    size_t framesPlayed,framesRemaining;

    VirtualDevice* connectedDevice;

    int fadeInFrames,fadeOutFrames;
    ma_uint64 totalFrames;
};

void applyFade( Uint8* buffer,const size_t bufferLength,const SDL_AudioSpec spec,
                int fadeInFrames,int fadeOutFrames,
                const bool fadeOutFromStart = false,
                const int pcurrentFrame = 0,const int totalFrames = 0,
                const bool audio = false, const bool audioStream = false);
void setVolume(Uint8* srcBuffer,size_t srcLength,Uint8* dstBuffer,float volume,SDL_AudioSpec spec);
void stream();

void _bind(py::module_& module);

} // namespace mixer
