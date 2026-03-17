import pytest

from pykraken import Vec2, Transform, transform


class TestTransformConstruction:
    def test_default(self):
        t = Transform()
        assert t.pos == Vec2()
        assert t.angle == 0.0
        assert t.scale == Vec2(1, 1)

    def test_with_pos(self):
        t = Transform(pos=Vec2(10, 20))
        assert t.pos == Vec2(10, 20)

    def test_with_angle(self):
        t = Transform(angle=1.5)
        assert t.angle == pytest.approx(1.5)

    def test_with_scale_float(self):
        t = Transform(scale=2.0)
        assert t.scale == Vec2(2, 2)

    def test_with_scale_vec(self):
        t = Transform(scale=Vec2(2, 3))
        assert t.scale == Vec2(2, 3)


class TestTransformFields:
    def test_readwrite(self):
        t = Transform()
        t.pos = Vec2(5, 5)
        t.angle = 3.14
        t.scale = Vec2(2, 2)
        assert t.pos == Vec2(5, 5)
        assert t.angle == pytest.approx(3.14)
        assert t.scale == Vec2(2, 2)


class TestCompose:
    def test_compose_identity(self):
        parent = Transform(pos=Vec2(10, 0))
        child = Transform()  # identity
        result = transform.compose(parent, child)
        assert result.pos.x == pytest.approx(10)
        assert result.pos.y == pytest.approx(0)

    def test_compose_translation(self):
        parent = Transform(pos=Vec2(10, 0))
        child = Transform(pos=Vec2(5, 0))
        result = transform.compose(parent, child)
        assert result.pos.x == pytest.approx(15)

    def test_compose_requires_two(self):
        with pytest.raises((ValueError, TypeError)):
            transform.compose(Transform())

    def test_compose_chain(self):
        a = Transform(pos=Vec2(10, 0))
        b = Transform(pos=Vec2(5, 0))
        c = Transform(pos=Vec2(3, 0))
        chain = transform.compose_chain(a, b, c)
        assert len(chain) >= 2
        # Last element should be full composition
        assert chain[-1].pos.x == pytest.approx(18)
