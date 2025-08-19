#include "Mixer.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace mixer
{


void _bind(pybind11::module_& module)
{
    auto subMixer = module.def_submodule("mixer", "mixer related functions");
    subMixer.def("init", &init,"Initializes the mixer");
    subMixer.def("quit", &quit, "kills the mixer");
    subMixer.def("stream", &stream, "streams the mixer [audioStreams");


    py::class_<mixer::Audio, std::shared_ptr<mixer::Audio>>(subMixer, "Audio")
        .def(py::init<const std::string&,mixer::VirtualDevice&, const float>(),
            py::arg("filepath"),
            py::arg("device"),
            py::arg("volume") = 1.0f
            )
        .def("start", &mixer::Audio::start,
            py::arg("fadein") = 0,
            py::arg("fadeout") = 0,
            "starts the audio from the beginning"
            )
        .def("stop", &mixer::Audio::stop,
            py::arg("fadeout") = 0,
            "completely stops the audio"
            )
        .def("length", &mixer::Audio::length,"returns length of the audio in seconds(int value)")
        .def("ended", &mixer::Audio::ended, "return a bool of whether or not the audio is playing")
        .def("load",&mixer::Audio::load, py::arg("filepath"), "can be used to change what audio the instance has been playing")
        .def_readwrite("volume",&mixer::Audio::volume);

    py::class_<mixer::AudioStream, std::shared_ptr<mixer::AudioStream>>(subMixer, "AudioStream")
        .def(py::init<const std::string&,mixer::VirtualDevice&, const float>(),
            py::arg("filepath"),
            py::arg("device"),
            py::arg("volume") = 1.0f
            )
        .def("play", &mixer::AudioStream::play,
            py::arg("fadein") = 0,
            py::arg("fadeout") = 0,
            py::arg("refadein") = false,
            "plays the audio from where it was left of, can be refaded in if need be")
        .def("pause", &mixer::AudioStream::pause,
            py::arg("fadeout") = 0,
            "pauses the audio, can be faded out if need be"
            )
        .def("rewind", &mixer::AudioStream::rewind,"restarts the audio from the beninging hehe")
        .def("length", &mixer::AudioStream::length,"returns length of the audio in seconds(int value)")
        .def("ended", &mixer::AudioStream::ended,"return a bool of whether or not the audio is playing")
        .def_readwrite("volume",&mixer::AudioStream::volume);

    py::class_<mixer::VirtualDevice>(subMixer, "VirtualDevice")
        .def(py::init<>())
        .def("play", &mixer::VirtualDevice::play)// these 2 start and stop the audio device
        .def("pause", &mixer::VirtualDevice::pause);
}



SDL_AudioSpec __outspec;
std::vector<AudioStream*> __AudioStreams;



void init()
{
    if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
        std::cerr<<"Failed to initialize SDL audio subsystem:\n";
        throw std::runtime_error(SDL_GetError());
    }
    __outspec = {
        SDL_AUDIO_S16,
        2,
        44100
    };
}
void quit()
{
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

VirtualDevice::VirtualDevice()
{
    cachedVolume = 1.0f;
    audioDevice = 0;
    audioDevice = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &__outspec);
    if (audioDevice == 0) {
        std::cerr<<"Failed to open audio device:\n";
        throw std::runtime_error(SDL_GetError());
    }
    SDL_ResumeAudioDevice(audioDevice);
}
VirtualDevice::~VirtualDevice()
{
    if(audioDevice != 0){
        SDL_CloseAudioDevice(audioDevice);
        audioDevice = 0;
    }
}
void VirtualDevice::play(){SDL_ResumeAudioDevice(audioDevice);}
void VirtualDevice::pause(){SDL_PauseAudioDevice(audioDevice);}
//add a way so that virtual device has a volume that affects all connected audio and audio device stream in the future

Audio::Audio(const std::string& filepath,VirtualDevice& device, const float volume)
:volume(volume),connectedDevice(&device)
{
    if(filepath!=""){
        if( !load(filepath) )
            throw std::runtime_error("Failed to load audio file");
    }
    if (connectedDevice)
        connectedDevice->connectedAudio.push_back(this);
}
Audio::~Audio()
{
    if (stream) {
        SDL_UnbindAudioStream(stream);
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }
    if (connectedDevice) {
        auto& vec = connectedDevice->connectedAudio;
        vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
    }
}
bool Audio::load(const std::string p_filepath)
{
    if (stream){
        SDL_ClearAudioStream(stream);
        SDL_UnbindAudioStream(stream);
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }

    void* buffer = nullptr;
    ma_uint64 framesRead = 0;
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, __outspec.channels, __outspec.freq);
    spec = __outspec;
    spec.format = SDL_AUDIO_F32;

    if (ma_decode_file(p_filepath.c_str(), &config, &framesRead, &buffer) != MA_SUCCESS) {
        return false;
    }

    size_t length = framesRead * 2 * sizeof(float);
    Uint8* rawdata = (Uint8*)buffer;

    stream = SDL_CreateAudioStream(
        &spec,
        &__outspec
    );
    if (!stream){
        std::cerr<<"failed to create audio stream:\n" ;
        throw std::runtime_error(SDL_GetError());
    }

    SDL_BindAudioStream(connectedDevice->audioDevice, stream);

    rawData.assign(rawdata, rawdata + length);
    ma_free(buffer, nullptr);
    return true;
}
void Audio::start(int fadeInSeconds,int fadeOutSeconds)
{
    if (!stream) return;
    std::vector<Uint8> data(rawData.size());
    setVolume(rawData.data(),rawData.size(),data.data(),volume,spec);
    if(fadeInSeconds > 0 ||fadeOutSeconds > 0){
        applyFade(data.data(),data.size(),spec,static_cast<int>(fadeInSeconds*spec.freq),static_cast<int>(fadeOutSeconds*spec.freq),false,0,0,true);
    }
    SDL_ClearAudioStream(stream);
    SDL_PutAudioStreamData(stream, data.data(), data.size());
    SDL_FlushAudioStream(stream);
}
void Audio::stop(int fadeOutSeconds)
{
    if (!stream) return;

    //replacement for now
    SDL_ClearAudioStream(stream);

    //// annoying code (this one actually works but issue is the same, sdl is not givin me the right spot so it wont fade out from the right spot)

    // if (fadeOutSeconds <= 0 || SDL_GetAudioStreamAvailable(stream) == 0){
    //     SDL_ClearAudioStream(stream);
    //     return;
    // }
    // size_t fadeOutBytes = fadeOutSeconds * spec.freq * spec.channels * (SDL_AUDIO_BITSIZE(spec.format) / 8);
    // size_t available = SDL_GetAudioStreamAvailable(stream);     //<<---// issues is here, it gets available bytes bases on what sdl has not what you have
    //                                                                  //   SDL_GetAudioStreamData() just gives data based on what sdl has done,
    //                                                                  //   this code works, but doesnt start at the right spot sadly
    // if(available > rawData.size()){
    //     available = rawData.size();
    // }
    // size_t start = rawData.size() - available;

    // std::vector<Uint8> data(rawData.data()+start,rawData.data()+start+fadeOutBytes);
    // applyFade(data.data(),data.size(),spec,0,static_cast<int>(fadeOutSeconds * spec.freq),true,0,0,true);

    // SDL_ClearAudioStream(stream);
    // SDL_PutAudioStreamData(stream, data.data(), data.size());
    // SDL_FlushAudioStream(stream);
}
int Audio::length()
{
    if (SDL_AUDIO_BITSIZE(spec.format) / 8 == 0 || spec.channels == 0 || spec.freq == 0) return 0;
    return ( static_cast<int>(rawData.size()) / (SDL_AUDIO_BITSIZE(spec.format) / 8 * spec.channels * spec.freq) );
}
bool Audio::ended(){return stream ? SDL_GetAudioStreamAvailable(stream) == 0 : true;}

MAFileDecoder::MAFileDecoder(const std::string& path)
{
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, __outspec.channels, __outspec.freq);

    if (ma_decoder_init_file(path.c_str(), &config, &decoder) != MA_SUCCESS) {
        std::cerr<<"Failed to open audio file with miniaudio.\n";
    }
    spec = __outspec;
    spec.format = SDL_AUDIO_F32;
}
MAFileDecoder::~MAFileDecoder()
{
    ma_decoder_uninit(&decoder);
}
size_t MAFileDecoder::read(Uint8* buffer,const size_t numBytes)
{
    if (!buffer || numBytes == 0) return 0;
    size_t framesToRead = numBytes / (decoder.outputChannels * sizeof(decoder.outputFormat) ); // decoder.outputChannels * sizeof(decoder.outputFormat)--> bytes per frame
    ma_uint64 framesRead;ma_decoder_read_pcm_frames(&decoder, buffer, framesToRead,&framesRead);
    return static_cast<size_t>(framesRead* decoder.outputChannels * sizeof(decoder.outputFormat) );
}
void MAFileDecoder::rewind(){ma_decoder_seek_to_pcm_frame(&decoder, 0);}



AudioStream::AudioStream(const std::string& filepath,VirtualDevice& device,const float volume)
:volume(volume), connectedDevice(&device),playing(false),stream(nullptr),audioDecoder(MAFileDecoder(filepath)),grabbedData(std::vector<Uint8>(DefaultStreamBufferSize))
{
    if (ma_decoder_get_length_in_pcm_frames(&audioDecoder.decoder, &totalFrames) != MA_SUCCESS)
        throw std::runtime_error("couldnt get total audioDecoder frames");
    framesRemaining = totalFrames;
    stream = SDL_CreateAudioStream(
        &audioDecoder.spec,
        &__outspec
    );
    spec = audioDecoder.spec;
    if (!stream){
        std::cerr<<"failed to create audio stream:\n" ;
        throw std::runtime_error(SDL_GetError());
    }

    SDL_BindAudioStream(connectedDevice->audioDevice, stream);
    if (connectedDevice) connectedDevice->connectedStreams.push_back(this);
    __AudioStreams.push_back(this);
}
AudioStream::~AudioStream()
{
    __AudioStreams.erase(std::remove(__AudioStreams.begin(), __AudioStreams.end(), this), __AudioStreams.end());

    if (stream) {
        SDL_UnbindAudioStream(stream);
        SDL_DestroyAudioStream(stream);
        stream = nullptr;
    }

    if (connectedDevice) {
        auto& vec = connectedDevice->connectedStreams;
        vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
    }
}
void AudioStream::play(const int fadeInSeconds,const int fadeOutSeconds,const bool reFadeIn)
{
    if(playing)return;
    playing = true;
    fadeInFrames = fadeInSeconds*spec.freq*spec.channels;
    fadeOutFrames = fadeOutSeconds*spec.freq*spec.channels;
    if (reFadeIn){
        totalFrames = framesRemaining;
        framesPlayed = 0;
    }
}
void AudioStream::pause(const int fadeOutSeconds)
{
    if(!playing)return;
    playing = false;
    size_t fadeOutBytes = fadeOutSeconds * spec.freq * spec.channels * (SDL_AUDIO_BITSIZE(spec.format) / 8) ;

    std::vector<Uint8> rdata(fadeOutBytes);
    audioDecoder.read(rdata.data(), fadeOutBytes);
    std::vector<Uint8> data(fadeOutBytes);

    setVolume(rdata.data(),fadeOutBytes,data.data(),volume,spec);
    applyFade(data.data(),data.size(),spec,0,static_cast<int>(fadeOutSeconds * spec.freq),true,0,0,true);

    SDL_PutAudioStreamData(stream, data.data(), data.size());
    SDL_FlushAudioStream(stream);
}
void AudioStream::rewind()
{
    audioDecoder.rewind();
    if (ma_decoder_get_length_in_pcm_frames(&audioDecoder.decoder, &totalFrames) != MA_SUCCESS)
        throw std::runtime_error("couldnt get total audioDecoder frames");
    framesRemaining = totalFrames;
    framesPlayed = 0;
}
int AudioStream::length(){ return static_cast<int>(totalFrames/spec.freq);}
bool AudioStream::ended(){return framesPlayed >= totalFrames;}
void AudioStream::__update__()
{
    if (!stream || !playing) return;
    int available = SDL_GetAudioStreamAvailable(stream);
    if (available < StreamBufferThreshold && playing) {
        size_t bytesRead = audioDecoder.read(grabbedData.data(), grabbedData.size());

        if (bytesRead > 0){
            int remainingFadeIn = std::max<int>(0, fadeInFrames - static_cast<int>(framesPlayed) );
            int remainingFadeOut = std::max<int>(0, fadeOutFrames - (totalFrames - static_cast<int>(framesPlayed)) );
            std::vector<Uint8> data(grabbedData.size());
            setVolume(grabbedData.data(),grabbedData.size(),data.data(),volume,spec);
            if (remainingFadeIn > 0 || remainingFadeOut > 0){
                applyFade(data.data(),data.size(),spec,remainingFadeIn,remainingFadeOut,false,framesPlayed,totalFrames ,false, true);
            }

            SDL_PutAudioStreamData(stream, data.data(), bytesRead);
            framesPlayed +=  bytesRead / (SDL_AUDIO_BITSIZE(spec.format)/8 * 2);
            framesRemaining -= bytesRead / (SDL_AUDIO_BITSIZE(spec.format)/8 * 2);
        }
    }

    if (framesPlayed >= totalFrames){
        playing = false;
        rewind();
    }
}



void setVolume(Uint8* srcBuffer,size_t srcLength,Uint8* dstBuffer,float volume,SDL_AudioSpec spec)
{
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    SDL_MixAudio(dstBuffer, srcBuffer, spec.format, srcLength, volume);
}
void applyFade( Uint8* buffer,const size_t bufferLength,const SDL_AudioSpec spec,
                int fadeInFrames,int fadeOutFrames,
                const bool fadeOutFromStart,
                const int pcurrentFrame,const int totalFrames,
                const bool audio, const bool audioStream )
{
// sample is audio data in one channel, frame includes both to all channels
// i need to add ease, idk when ill do goodluck tho whenver i do it
    if (audio){
        float* samples = reinterpret_cast<float*>(buffer);
        size_t FrameCount = bufferLength/(SDL_AUDIO_BITSIZE(spec.format) / 8 * spec.channels);
        if(fadeInFrames > 0){
            fadeInFrames = std::min<size_t>(fadeInFrames, FrameCount);
            for (size_t i = 0; i < fadeInFrames; ++i) {
                float gain = static_cast<float>(i) / fadeInFrames;
                for (int ch = 0; ch < spec.channels; ++ch){
                    samples[i * spec.channels + ch] *= gain;
                }
            }
        }
        if(fadeOutFrames > 0){
            fadeOutFrames = std::min<size_t>(fadeOutFrames, FrameCount);
            size_t start = fadeOutFromStart ? 0 : FrameCount - fadeOutFrames;
            for (size_t i = 0; i < fadeOutFrames; ++i) {
                float gain = 1.0f - static_cast<float>(i) / fadeOutFrames;
                for (int ch = 0; ch < spec.channels; ++ch){
                    samples[i * spec.channels + ch] *= gain;
                }
            }
        }
    }
    if (audioStream){
        float* samples = reinterpret_cast<float*>(buffer);
        size_t FrameCount = bufferLength / (sizeof(float) * 2);
        for (size_t i = 0; i < FrameCount; ++i) {
            int currentFrame = pcurrentFrame + static_cast<int>(i);
            float gain = 1.0f;
            if (currentFrame < fadeInFrames) {
                gain = static_cast<float>(currentFrame) / fadeInFrames;
            }
            if (fadeOutFrames > 0) {
                if (currentFrame >= (totalFrames - fadeOutFrames) && currentFrame < totalFrames) {
                    float fadeOutProgress = static_cast<float>(currentFrame - (totalFrames - fadeOutFrames)) / fadeOutFrames;
                    gain *= 1.0f - fadeOutProgress;
                }
            }

            for (int ch = 0; ch < spec.channels; ++ch){
                samples[i * spec.channels + ch] *= gain;
            }
        }
    }
}
void stream()
{
    for (auto audiostream : __AudioStreams) {
        if (audiostream) audiostream->__update__();
    }
}
} // namespace mixer
