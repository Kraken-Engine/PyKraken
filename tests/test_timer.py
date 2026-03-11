import time as systime

import pytest

from pykraken import Timer


class TestTimerConstruction:
    def test_valid_duration(self):
        t = Timer(1.0)
        assert not t.done

    def test_zero_duration_raises(self):
        with pytest.raises(ValueError):
            Timer(0.0)

    def test_negative_duration_raises(self):
        with pytest.raises(ValueError):
            Timer(-1.0)


class TestTimerBeforeStart:
    def test_not_done(self):
        t = Timer(1.0)
        assert not t.done

    def test_progress_zero(self):
        t = Timer(1.0)
        assert t.progress == pytest.approx(0.0)

    def test_time_remaining(self):
        t = Timer(5.0)
        assert t.time_remaining == pytest.approx(5.0)

    def test_elapsed_zero(self):
        t = Timer(1.0)
        assert t.elapsed_time == pytest.approx(0.0)


class TestTimerRunning:
    def test_completes(self):
        t = Timer(0.05)
        t.start()
        systime.sleep(0.08)
        assert t.done
        assert t.progress == pytest.approx(1.0)

    def test_not_done_early(self):
        t = Timer(1.0)
        t.start()
        systime.sleep(0.01)
        assert not t.done
        assert t.progress > 0.0
        assert t.progress < 1.0


class TestTimerPauseResume:
    def test_pause_freezes_progress(self):
        t = Timer(1.0)
        t.start()
        systime.sleep(0.05)
        t.pause()
        p1 = t.progress
        systime.sleep(0.05)
        p2 = t.progress
        assert p1 == pytest.approx(p2, abs=0.01)

    def test_resume_continues(self):
        t = Timer(0.1)
        t.start()
        systime.sleep(0.03)
        t.pause()
        t.resume()
        systime.sleep(0.1)
        assert t.done


class TestTimerReset:
    def test_reset_restarts(self):
        t = Timer(0.05)
        t.start()
        systime.sleep(0.08)
        assert t.done
        t.reset()
        assert not t.done
        assert t.progress == pytest.approx(0.0)
