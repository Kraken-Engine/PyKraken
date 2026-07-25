#include "kraken/audio/Mixer.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "bindings/python/bindings.hpp"
#include "kraken/core/Log.hpp"

namespace kn::mixer
{
void _bind(nb::module_& module)
{
    using namespace nb::literals;

    auto subMixer = module.def_submodule("mixer", R"doc(
        Sound mixer and audio management system.

        The mixer manages a pool of 64 mixer tracks/voices for playing
        :class:`Sample` (short polyphonic sounds) and :class:`Stream`
        (long music files). It handles automatic track acquisition and
        priority-based sound stealing when the track pool is exhausted.
    )doc");

    nb::enum_<AudioPriority>(subMixer, "AudioPriority", R"doc(
        Priority levels used for track acquisition.

        Used to determine which sounds to interrupt ('steal') when the 64-track
        limit is reached. Higher priority sounds are more protected from being stolen.
    )doc")
        .value("MUSIC", AudioPriority::Music, "Highest priority level.")
        .value("UI", AudioPriority::UI, "Medium priority level.")
        .value("SFX", AudioPriority::SFX, "Standard priority level.");

    nb::class_<Audio>(subMixer, "Audio", R"doc(
        Abstract base class for all audio resources.

        Common interface for local volume and playback status. Local volume
        is multiplied by the mixer's master volume. Both default to 1.0.

        Attributes:
            volume (float): Local volume multiplier (0.0 to 1.0). Defaults to 1.0.
            playing (bool): (Read-only) Whether the audio is currently playing on any track.

        Methods:
            play(fade_in=0.0): Start audio playback.
            stop(fade_out=0.0): Stop all instances of this audio resource.
    )doc")
        .def_prop_rw("volume", &Audio::getVolume, &Audio::setVolume, "Volume scalar (0.0 to 1.0).")
        .def_prop_ro("playing", &Audio::isPlaying, "True if currently playing.")

        .def("play", &Audio::play, "fade_in"_a = 0.0, R"doc(
            Start audio playback.

            Args:
                fade_in (float): Fade in duration in seconds. Defaults to 0.0.
        )doc")
        .def("stop", &Audio::stop, "fade_out"_a = 0.0, R"doc(
            Stop all playing instances of this audio.

            Args:
                fade_out (float): Fade out duration in seconds. Defaults to 0.0.
        )doc");

    nb::class_<Sample, Audio>(subMixer, "Sample", R"doc(
        A sound effect sample loaded entirely into memory.

        Samples support polyphony (multiple simultaneous instances). If tracks
        are full, samples attempt to steal tracks from lower-priority or older sounds.

        Attributes:
            priority (AudioPriority): Acquisition priority level. Defaults to SFX.
            can_steal (bool): Whether this sound can interrupt others to acquire a
                track. Defaults to True.
            max_polyphony (int): Maximum simultaneous instances of this specific
                sample (Range 1-32). Defaults to 1.
    )doc")
        .def_rw("priority", &Sample::priority, "Acquisition priority level.")
        .def_rw("can_steal", &Sample::canSteal, "Whether can interrupt others to acquire a track.")
        .def_prop_rw(
            "max_polyphony", &Sample::getMaxPolyphony, &Sample::setMaxPolyphony,
            "Max simultaneous instances of sample (1-32)."
        );

    nb::class_<Stream, Audio>(subMixer, "Stream", R"doc(
        A streaming audio resource intended for long music files.

        Streams occupy exactly one track while active. They are protected and
        will not be stolen by incoming :class:`Sample` requests.

        Attributes:
            playback_pos (float): (Read-only) Current playback position in seconds.
            looping (bool): Whether the stream should loop when it reaches the end.

        Methods:
            pause(): Pause playback, preserving position.
            resume(fade_in=0.0): Resume playback from a paused state.
            seek(seconds): Jump to a specific time in the audio file.
    )doc")
        .def_prop_ro(
            "playback_pos", &Stream::getPlaybackPos,
            R"doc(Current position in seconds. 0.0 if stopped/never played, paused position if paused.)doc"
        )
        .def_prop_rw(
            "looping", &Stream::getLooping, &Stream::setLooping,
            R"doc(Whether the stream should loop when it reaches the end.)doc"
        )

        .def("pause", &Stream::pause, R"doc(
            Pause playback. Releases the hardware track but preserves position.
        )doc")
        .def("resume", &Stream::resume, "fade_in"_a = 0.0, R"doc(
            Resume playback from a paused state.

            Args:
                fade_in (float): Duration in seconds to fade back in. Defaults to 0.0.
        )doc")
        .def("seek", &Stream::seek, "seconds"_a, R"doc(
            Jump to a specific time in the audio file.

            Args:
                seconds (float): Target position in seconds from the start.
        )doc");

    subMixer.def(
        "load_sample", &loadSample, nb::call_guard<nb::gil_scoped_release>(), "path"_a,
        "predecode"_a = true, R"doc(
        Load an audio sample (SFX) from disk.

        Args:
            path (str | os.PathLike[str]): File path to load.
            predecode (bool): Whether to decode into memory now. Defaults to True.

        Returns:
            Sample: The loaded audio object.
        )doc"
    );

    subMixer.def(
        "load_stream", &loadStream, nb::call_guard<nb::gil_scoped_release>(), "path"_a,
        "predecode"_a = false, R"doc(
        Load an audio stream (Music) from disk.

        Args:
            path (str | os.PathLike[str]): File path to load.
            predecode (bool): Whether to decode into memory now. Defaults to False.

        Returns:
            Stream: The loaded audio object.
    )doc"
    );

    subMixer.def("set_master_volume", &setMasterVolume, "volume"_a, R"doc(
        Set the global mixer gain.

        This affects all playing samples and streams. Individual audio volume
        is multiplied by this value. Default is 1.0.

        Args:
            volume (float): Master volume scalar (0.0 to 1.0).
    )doc");

    subMixer.def("get_master_volume", &getMasterVolume, R"doc(
        Get the current global mixer gain.

        Returns:
            float: Master volume (0.0 to 1.0).
    )doc");
}
}  // namespace kn::mixer
