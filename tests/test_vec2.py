import math as pymath

import pytest

from pykraken import Vec2, PolarCoordinate


class TestVec2Construction:
    def test_default(self):
        v = Vec2()
        assert v.x == 0.0 and v.y == 0.0

    def test_single_value(self):
        v = Vec2(5.0)
        assert v.x == 5.0 and v.y == 5.0

    def test_two_values(self):
        v = Vec2(3.0, 4.0)
        assert v.x == 3.0 and v.y == 4.0


class TestVec2Properties:
    def test_length(self):
        assert Vec2(3, 4).length == pytest.approx(5.0)

    def test_length_squared(self):
        assert Vec2(3, 4).length_squared == pytest.approx(25.0)

    def test_angle(self):
        assert Vec2(1, 0).angle == pytest.approx(0.0)
        assert Vec2(0, 1).angle == pytest.approx(pymath.pi / 2)

    def test_swizzles(self):
        v = Vec2(2, 7)
        assert v.xx == Vec2(2, 2)
        assert v.yy == Vec2(7, 7)
        assert v.xy == Vec2(2, 7)
        assert v.yx == Vec2(7, 2)


class TestVec2StaticConstants:
    def test_zero(self):
        assert Vec2.ZERO == Vec2(0, 0)

    def test_directions(self):
        assert Vec2.UP == Vec2(0, -1)
        assert Vec2.DOWN == Vec2(0, 1)
        assert Vec2.LEFT == Vec2(-1, 0)
        assert Vec2.RIGHT == Vec2(1, 0)


class TestVec2Methods:
    def test_copy(self):
        v = Vec2(1, 2)
        c = v.copy()
        assert c == v
        c.x = 99
        assert v.x == 1

    def test_is_zero(self):
        assert Vec2().is_zero()
        assert not Vec2(1, 0).is_zero()
        assert Vec2(1e-9, 0).is_zero()
        assert not Vec2(1e-9, 0).is_zero(tolerance=1e-10)

    def test_normalize(self):
        v = Vec2(3, 4)
        v.normalize()
        assert v.length == pytest.approx(1.0)

    def test_normalized(self):
        v = Vec2(3, 4)
        n = v.normalized()
        assert n.length == pytest.approx(1.0)
        assert v == Vec2(3, 4)  # original unchanged

    def test_normalize_zero_vector(self):
        v = Vec2()
        v.normalize()
        assert v == Vec2()

    def test_rotate(self):
        v = Vec2(1, 0)
        v.rotate(pymath.pi / 2)
        assert v.x == pytest.approx(0.0, abs=1e-10)
        assert v.y == pytest.approx(1.0)

    def test_rotated(self):
        v = Vec2(1, 0)
        r = v.rotated(pymath.pi)
        assert r.x == pytest.approx(-1.0)
        assert r.y == pytest.approx(0.0, abs=1e-10)
        assert v == Vec2(1, 0)

    def test_scale_to_length(self):
        v = Vec2(3, 4)
        v.scale_to_length(10.0)
        assert v.length == pytest.approx(10.0)

    def test_scaled_to_length(self):
        v = Vec2(3, 4)
        s = v.scaled_to_length(10.0)
        assert s.length == pytest.approx(10.0)
        assert v.length == pytest.approx(5.0)

    def test_project(self):
        v = Vec2(3, 4)
        onto = Vec2(1, 0)
        p = v.project(onto)
        assert p == Vec2(3, 0)

    def test_reject(self):
        v = Vec2(3, 4)
        onto = Vec2(1, 0)
        r = v.reject(onto)
        assert r.x == pytest.approx(0.0)
        assert r.y == pytest.approx(4.0)

    def test_reflect(self):
        v = Vec2(1, -1)
        normal = Vec2(0, 1)
        r = v.reflect(normal)
        assert r.x == pytest.approx(1.0)
        assert r.y == pytest.approx(1.0)

    def test_distance_to(self):
        a = Vec2(0, 0)
        b = Vec2(3, 4)
        assert a.distance_to(b) == pytest.approx(5.0)

    def test_distance_squared_to(self):
        a = Vec2(0, 0)
        b = Vec2(3, 4)
        assert a.distance_squared_to(b) == pytest.approx(25.0)

    def test_to_polar(self):
        v = Vec2(1, 0)
        p = v.to_polar()
        assert isinstance(p, PolarCoordinate)
        assert p.angle == pytest.approx(0.0)
        assert p.radius == pytest.approx(1.0)

    def test_move_toward(self):
        v = Vec2(0, 0)
        v.move_toward(Vec2(10, 0), 3.0)
        assert v.x == pytest.approx(3.0)
        assert v.y == pytest.approx(0.0)

    def test_move_toward_overshoot(self):
        v = Vec2(0, 0)
        v.move_toward(Vec2(2, 0), 10.0)
        assert v == Vec2(2, 0)

    def test_moved_toward(self):
        v = Vec2(0, 0)
        m = v.moved_toward(Vec2(10, 0), 3.0)
        assert m.x == pytest.approx(3.0)
        assert v == Vec2(0, 0)

    def test_floored(self):
        assert Vec2(1.7, -0.3).floored() == Vec2(1.0, -1.0)

    def test_ceiled(self):
        assert Vec2(1.2, -0.7).ceiled() == Vec2(2.0, 0.0)

    def test_rounded(self):
        assert Vec2(1.4, 1.6).rounded() == Vec2(1.0, 2.0)

    def test_as_ints(self):
        result = Vec2(3.7, -1.2).as_ints()
        assert result == (3, -1)


class TestVec2Operators:
    def test_add(self):
        assert Vec2(1, 2) + Vec2(3, 4) == Vec2(4, 6)

    def test_sub(self):
        assert Vec2(5, 3) - Vec2(1, 1) == Vec2(4, 2)

    def test_iadd(self):
        v = Vec2(1, 2)
        v += Vec2(3, 4)
        assert v == Vec2(4, 6)

    def test_neg(self):
        assert -Vec2(1, -2) == Vec2(-1, 2)

    def test_mul_scalar(self):
        assert Vec2(2, 3) * 2 == Vec2(4, 6)

    def test_rmul_scalar(self):
        assert 2 * Vec2(2, 3) == Vec2(4, 6)

    def test_mul_vec(self):
        assert Vec2(2, 3) * Vec2(4, 5) == Vec2(8, 15)

    def test_div_scalar(self):
        assert Vec2(6, 4) / 2 == Vec2(3, 2)

    def test_div_vec(self):
        assert Vec2(6, 4) / Vec2(3, 2) == Vec2(2, 2)

    def test_equality(self):
        assert Vec2(1, 2) == Vec2(1, 2)
        assert Vec2(1, 2) != Vec2(1, 3)

    def test_bool_nonzero(self):
        assert bool(Vec2(1, 0))

    def test_bool_zero(self):
        assert not bool(Vec2(0, 0))

    def test_indexing(self):
        v = Vec2(5, 10)
        assert v[0] == 5
        assert v[1] == 10

    def test_setitem(self):
        v = Vec2()
        v[0] = 7
        v[1] = 8
        assert v == Vec2(7, 8)

    def test_len(self):
        assert len(Vec2()) == 2

    def test_iter(self):
        assert list(Vec2(3, 4)) == [3.0, 4.0]

    def test_hash(self):
        assert hash(Vec2(1, 2)) == hash(Vec2(1, 2))


class TestPolarCoordinate:
    def test_default(self):
        p = PolarCoordinate()
        assert p.angle == 0.0 and p.radius == 0.0

    def test_values(self):
        p = PolarCoordinate(pymath.pi, 5.0)
        assert p.angle == pytest.approx(pymath.pi)
        assert p.radius == pytest.approx(5.0)

    def test_to_cartesian(self):
        p = PolarCoordinate(0.0, 1.0)
        v = p.to_cartesian()
        assert v.x == pytest.approx(1.0)
        assert v.y == pytest.approx(0.0, abs=1e-10)

    def test_roundtrip(self):
        original = Vec2(3, 4)
        polar = original.to_polar()
        back = polar.to_cartesian()
        assert back.x == pytest.approx(original.x)
        assert back.y == pytest.approx(original.y)

    def test_equality(self):
        assert PolarCoordinate(1.0, 2.0) == PolarCoordinate(1.0, 2.0)
        assert PolarCoordinate(1.0, 2.0) != PolarCoordinate(1.0, 3.0)

    def test_indexing(self):
        p = PolarCoordinate(1.5, 3.0)
        assert p[0] == pytest.approx(1.5)
        assert p[1] == pytest.approx(3.0)

    def test_len(self):
        assert len(PolarCoordinate()) == 2
