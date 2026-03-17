import math as pymath

import pytest

from pykraken import Vec2, math


class TestMathFunctions:
    def test_from_polar(self):
        v = math.from_polar(0.0, 1.0)
        assert v.x == pytest.approx(1.0)
        assert v.y == pytest.approx(0.0, abs=1e-10)

    def test_from_polar_90deg(self):
        v = math.from_polar(pymath.pi / 2, 1.0)
        assert v.x == pytest.approx(0.0, abs=1e-10)
        assert v.y == pytest.approx(1.0)

    def test_dot(self):
        assert math.dot(Vec2(1, 0), Vec2(0, 1)) == pytest.approx(0.0)
        assert math.dot(Vec2(1, 0), Vec2(1, 0)) == pytest.approx(1.0)
        assert math.dot(Vec2(3, 4), Vec2(2, 1)) == pytest.approx(10.0)

    def test_cross(self):
        assert math.cross(Vec2(1, 0), Vec2(0, 1)) == pytest.approx(1.0)
        assert math.cross(Vec2(0, 1), Vec2(1, 0)) == pytest.approx(-1.0)

    def test_angle_between_perpendicular(self):
        assert math.angle_between(Vec2(1, 0), Vec2(0, 1)) == pytest.approx(pymath.pi / 2)

    def test_angle_between_same(self):
        assert math.angle_between(Vec2(1, 0), Vec2(1, 0)) == pytest.approx(0.0)

    def test_angle_between_opposite(self):
        assert math.angle_between(Vec2(1, 0), Vec2(-1, 0)) == pytest.approx(pymath.pi)


class TestClamp:
    def test_clamp_scalar(self):
        assert math.clamp(5.0, 0.0, 10.0) == 5.0
        assert math.clamp(-5.0, 0.0, 10.0) == 0.0
        assert math.clamp(15.0, 0.0, 10.0) == 10.0

    def test_clamp_vec(self):
        v = math.clamp(Vec2(15, -5), Vec2(0, 0), Vec2(10, 10))
        assert v == Vec2(10, 0)


class TestLerp:
    def test_lerp_float(self):
        assert math.lerp(0.0, 10.0, 0.5) == pytest.approx(5.0)
        assert math.lerp(0.0, 10.0, 0.0) == pytest.approx(0.0)
        assert math.lerp(0.0, 10.0, 1.0) == pytest.approx(10.0)

    def test_lerp_vec2(self):
        result = math.lerp(Vec2(0, 0), Vec2(10, 20), 0.5)
        assert result.x == pytest.approx(5.0)
        assert result.y == pytest.approx(10.0)


class TestRemap:
    def test_remap_basic(self):
        assert math.remap(0, 10, 0, 100, 5) == pytest.approx(50.0)

    def test_remap_zero_range_raises(self):
        with pytest.raises(ValueError):
            math.remap(5, 5, 0, 100, 5)

    def test_remap_reverse(self):
        assert math.remap(0, 10, 100, 0, 5) == pytest.approx(50.0)


class TestConstants:
    def test_deg2rad(self):
        assert math.DEG2RAD == pytest.approx(pymath.pi / 180)

    def test_rad2deg(self):
        assert math.RAD2DEG == pytest.approx(180 / pymath.pi)
