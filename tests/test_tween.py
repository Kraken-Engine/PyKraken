import time as systime

import pykraken
import pytest

from pykraken import Tween, Vec2, ease, time as ktime, window


def _pump_frames(frame_count=10, sleep_s=0.01):
    for _ in range(frame_count):
        window.is_open()
        systime.sleep(sleep_s)


class TestTweenAutoTick:
    def test_auto_updates_without_manual_step(self):
        pykraken.init()
        try:
            ktime.set_max_delta(0.01)
            tween = Tween(ease.linear, 0.05)
            tween.start_pos = Vec2(0.0, 0.0)
            tween.end_pos = Vec2(10.0, 0.0)

            assert tween.current_pos.x == pytest.approx(0.0)

            _pump_frames(frame_count=3, sleep_s=0.01)
            mid_x = tween.current_pos.x
            assert 0.0 < mid_x < 10.0

            _pump_frames(frame_count=10, sleep_s=0.01)
            end = tween.current_pos
            assert end.x == pytest.approx(10.0, abs=0.5)
            assert end.y == pytest.approx(0.0, abs=0.1)
            assert tween.is_done
        finally:
            pykraken.quit()

    def test_pause_holds_position_until_resume(self):
        pykraken.init()
        try:
            ktime.set_max_delta(0.01)
            tween = Tween(ease.linear, 0.2)
            tween.start_pos = Vec2(0.0, 0.0)
            tween.end_pos = Vec2(20.0, 0.0)

            _pump_frames(frame_count=4, sleep_s=0.01)
            tween.pause()
            paused_x = tween.current_pos.x

            _pump_frames(frame_count=6, sleep_s=0.01)
            assert tween.current_pos.x == pytest.approx(paused_x, abs=0.2)

            tween.resume()
            _pump_frames(frame_count=8, sleep_s=0.01)
            assert tween.current_pos.x > paused_x
        finally:
            pykraken.quit()
