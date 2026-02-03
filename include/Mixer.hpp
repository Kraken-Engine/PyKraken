#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <pybind11/pybind11.h>

#include <memory>
#include <string>

namespace py = pybind11;

namespace kn
{
namespace mixer
{
class Audio;
class Sample;
class Stream;

void _bind(py::module_& module);
void _init();
void _quit();

std::shared_ptr<Sample> loadSample(const std::string& path, bool predecode = true);
std::shared_ptr<Stream> loadStream(const std::string& path, bool predecode = false);
void setMasterVolume(float volume);
float getMasterVolume();

enum class AudioPriority : uint8_t
{
    Music = 0,
    UI = 2,
    SFX = 6,
};

class Audio
{
  public:
    AudioPriority priority = AudioPriority::SFX;
    bool canSteal = true;

    virtual ~Audio();

    virtual void setVolume(float volume) = 0;
    float getVolume() const;

    virtual bool isPlaying() const = 0;

    virtual void play(double fadeInSeconds = 0.0) = 0;
    virtual void stop(double fadeOutSeconds = 0.0) = 0;

  protected:
    MIX_Audio* m_audio = nullptr;
    bool m_looping = false;
    float m_volume = 1.0f;

    explicit Audio(MIX_Audio* sdlAudio);
};

class Sample : public Audio
{
  public:
    ~Sample() override = default;

    void setMaxPolyphony(uint8_t maxPolyphony);
    uint8_t getMaxPolyphony() const;

    void setVolume(float volume) override;

    bool isPlaying() const override;

    void play(double fadeInSeconds = 0.0) override;
    void stop(double fadeOutSeconds = 0.0) override;

  private:
    uint8_t m_maxPolyphony = 1;

    explicit Sample(MIX_Audio* sdlAudio);

    friend std::shared_ptr<Sample> loadSample(const std::string& path, bool predecode);
};

class Stream : public Audio
{
  public:
    Stream(MIX_Audio* sdlAudio);
    ~Stream() override;

    void setLooping(bool looping);
    bool getLooping() const;
    void setVolume(float volume) override;

    bool isPlaying() const override;
    double getPlaybackPos() const;

    void play(double fadeInSeconds = 0.0) override;
    void pause();
    void resume(double fadeInSeconds = 0.0);
    void stop(double fadeOutSeconds = 0.0) override;
    void seek(double seconds);

  private:
    int m_trackIndex = -1;     // -1 means not attached to a track (stopped/paused)
    Sint64 m_savedFrames = 0;  // remembered playback position when paused

    friend std::shared_ptr<Stream> loadStream(const std::string& path, bool predecode);
};
}  // namespace mixer
}  // namespace kn
