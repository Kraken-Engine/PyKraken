import pytest

from pykraken import Rect, Vec2


class TestRectConstruction:
    def test_default(self):
        r = Rect()
        assert (r.x, r.y, r.w, r.h) == (0, 0, 0, 0)

    def test_from_size_vec(self):
        r = Rect(Vec2(10, 20))
        assert r.x == 0 and r.y == 0 and r.w == 10 and r.h == 20

    def test_xywh(self):
        r = Rect(1, 2, 30, 40)
        assert (r.x, r.y, r.w, r.h) == (1, 2, 30, 40)

    def test_pos_and_size_vec(self):
        r = Rect(Vec2(5, 10), Vec2(20, 30))
        assert (r.x, r.y, r.w, r.h) == (5, 10, 20, 30)


class TestRectProperties:
    def test_edges(self):
        r = Rect(10, 20, 100, 50)
        assert r.left == 10
        assert r.right == 110
        assert r.top == 20
        assert r.bottom == 70

    def test_set_edges(self):
        r = Rect(0, 0, 100, 100)
        r.left = 50
        assert r.x == 50
        r.top = 30
        assert r.y == 30

    def test_center(self):
        r = Rect(0, 0, 100, 100)
        assert r.center.x == pytest.approx(50)
        assert r.center.y == pytest.approx(50)

    def test_set_center(self):
        r = Rect(0, 0, 100, 100)
        r.center = Vec2(200, 200)
        assert r.x == pytest.approx(150)
        assert r.y == pytest.approx(150)

    def test_size(self):
        r = Rect(5, 5, 20, 30)
        assert r.size == Vec2(20, 30)

    def test_corners(self):
        r = Rect(10, 20, 100, 50)
        assert r.top_left == Vec2(10, 20)
        assert r.top_right == Vec2(110, 20)
        assert r.bottom_left == Vec2(10, 70)
        assert r.bottom_right == Vec2(110, 70)
        assert r.top_mid.x == pytest.approx(60)
        assert r.bottom_mid.x == pytest.approx(60)
        assert r.mid_left.y == pytest.approx(45)
        assert r.mid_right.y == pytest.approx(45)


class TestRectMethods:
    def test_copy(self):
        r = Rect(1, 2, 3, 4)
        c = r.copy()
        assert c == r
        c.x = 99
        assert r.x == 1

    def test_move(self):
        r = Rect(0, 0, 10, 10)
        r.move(Vec2(5, 5))
        assert r.x == pytest.approx(5)
        assert r.y == pytest.approx(5)

    def test_inflate(self):
        r = Rect(10, 10, 100, 100)
        r.inflate(Vec2(20, 20))
        assert r.w == 120 and r.h == 120
        assert r.x == pytest.approx(0)  # centered inflation

    def test_fit(self):
        outer = Rect(0, 0, 200, 100)
        inner = Rect(0, 0, 50, 50)
        inner.fit(outer)
        # should be scaled to fit within outer while maintaining aspect ratio
        assert inner.w <= 200 and inner.h <= 100

    def test_clamp_inside(self):
        r = Rect(300, 300, 10, 10)
        bounds = Rect(0, 0, 100, 100)
        r.clamp(bounds)
        assert r.right <= bounds.right
        assert r.bottom <= bounds.bottom

    def test_scale_by(self):
        r = Rect(0, 0, 100, 50)
        r.scale_by(2.0)
        assert r.w == pytest.approx(200)
        assert r.h == pytest.approx(100)

    def test_scale_to(self):
        r = Rect(0, 0, 100, 50)
        r.scale_to(Vec2(50, 25))
        assert r.w == pytest.approx(50)
        assert r.h == pytest.approx(25)


class TestRectOperators:
    def test_equality(self):
        assert Rect(1, 2, 3, 4) == Rect(1, 2, 3, 4)
        assert Rect(1, 2, 3, 4) != Rect(1, 2, 3, 5)

    def test_bool_nonzero(self):
        assert bool(Rect(0, 0, 10, 10))

    def test_bool_zero(self):
        assert not bool(Rect())

    def test_indexing(self):
        r = Rect(1, 2, 3, 4)
        assert r[0] == 1
        assert r[1] == 2
        assert r[2] == 3
        assert r[3] == 4

    def test_iter(self):
        assert list(Rect(1, 2, 3, 4)) == [1.0, 2.0, 3.0, 4.0]

    def test_len(self):
        assert len(Rect()) == 4
