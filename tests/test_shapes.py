import pytest

from pykraken import Line, Circle, Capsule, Polygon, Vec2


# ──────────── Line ────────────


class TestLineConstruction:
    def test_default(self):
        l = Line()
        assert (l.ax, l.ay, l.bx, l.by) == (0, 0, 0, 0)

    def test_four_floats(self):
        l = Line(1, 2, 3, 4)
        assert (l.ax, l.ay, l.bx, l.by) == (1, 2, 3, 4)

    def test_two_vec2s(self):
        l = Line(Vec2(1, 2), Vec2(3, 4))
        assert l.a == Vec2(1, 2)
        assert l.b == Vec2(3, 4)


class TestLineProperties:
    def test_length(self):
        l = Line(0, 0, 3, 4)
        assert l.length == pytest.approx(5.0)

    def test_a_b_properties(self):
        l = Line(1, 2, 3, 4)
        assert l.a == Vec2(1, 2)
        assert l.b == Vec2(3, 4)

    def test_set_a_b(self):
        l = Line()
        l.a = Vec2(10, 20)
        l.b = Vec2(30, 40)
        assert l.ax == 10 and l.ay == 20
        assert l.bx == 30 and l.by == 40


class TestLineMethods:
    def test_copy(self):
        l = Line(1, 2, 3, 4)
        c = l.copy()
        assert c == l
        c.ax = 99
        assert l.ax == 1

    def test_move(self):
        l = Line(0, 0, 10, 10)
        l.move(Vec2(5, 5))
        assert l.a == Vec2(5, 5)
        assert l.b == Vec2(15, 15)


class TestLineOperators:
    def test_equality(self):
        assert Line(1, 2, 3, 4) == Line(1, 2, 3, 4)
        assert Line(1, 2, 3, 4) != Line(1, 2, 3, 5)

    def test_len(self):
        assert len(Line()) == 4


# ──────────── Circle ────────────


class TestCircleConstruction:
    def test_default(self):
        c = Circle()
        assert c.radius == 0 and c.pos == Vec2()

    def test_radius_only(self):
        c = Circle(5.0)
        assert c.radius == pytest.approx(5.0)
        assert c.pos == Vec2()

    def test_pos_and_radius(self):
        c = Circle(Vec2(3, 4), 5.0)
        assert c.pos == Vec2(3, 4)
        assert c.radius == pytest.approx(5.0)

    def test_xyz(self):
        c = Circle(3, 4, 5)
        assert c.pos == Vec2(3, 4)
        assert c.radius == pytest.approx(5)


class TestCircleProperties:
    def test_area(self):
        import math as pymath

        c = Circle(Vec2(0, 0), 1.0)
        assert c.area == pytest.approx(pymath.pi)

    def test_circumference(self):
        import math as pymath

        c = Circle(Vec2(0, 0), 1.0)
        assert c.circumference == pytest.approx(2 * pymath.pi)


class TestCircleMethods:
    def test_as_rect(self):
        c = Circle(Vec2(10, 10), 5)
        r = c.as_rect()
        assert r.w == pytest.approx(10) and r.h == pytest.approx(10)

    def test_copy(self):
        c = Circle(Vec2(1, 2), 3)
        c2 = c.copy()
        assert c == c2
        c2.radius = 99
        assert c.radius == 3

    def test_equality(self):
        assert Circle(Vec2(1, 2), 3) == Circle(Vec2(1, 2), 3)
        assert Circle(Vec2(1, 2), 3) != Circle(Vec2(1, 2), 4)


# ──────────── Capsule ────────────


class TestCapsuleConstruction:
    def test_default(self):
        c = Capsule()
        assert c.p1 == Vec2() and c.p2 == Vec2() and c.radius == 0

    def test_vec2s_and_radius(self):
        c = Capsule(Vec2(0, 0), Vec2(10, 0), 5.0)
        assert c.p1 == Vec2(0, 0)
        assert c.p2 == Vec2(10, 0)
        assert c.radius == pytest.approx(5.0)


class TestCapsuleMethods:
    def test_as_rect(self):
        c = Capsule(Vec2(0, 0), Vec2(10, 0), 5.0)
        r = c.as_rect()
        assert r.w > 0 and r.h > 0

    def test_copy(self):
        c = Capsule(Vec2(0, 0), Vec2(10, 0), 5.0)
        c2 = c.copy()
        assert c == c2
        c2.radius = 99
        assert c.radius == pytest.approx(5.0)

    def test_equality(self):
        a = Capsule(Vec2(0, 0), Vec2(10, 0), 5.0)
        b = Capsule(Vec2(0, 0), Vec2(10, 0), 5.0)
        assert a == b


# ──────────── Polygon ────────────


class TestPolygonConstruction:
    def test_default(self):
        p = Polygon()
        assert len(p.points) == 0

    def test_from_points(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert len(p.points) == 4


class TestPolygonProperties:
    def test_area(self):
        # 10x10 square
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert p.area == pytest.approx(100.0)

    def test_perimeter(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert p.perimeter == pytest.approx(40.0)

    def test_centroid(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert p.centroid.x == pytest.approx(5.0)
        assert p.centroid.y == pytest.approx(5.0)

    def test_convex_square(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert p.is_convex
        assert not p.is_concave

    def test_concave_l_shape(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 5), Vec2(5, 5), Vec2(5, 10), Vec2(0, 10)]
        p = Polygon(pts)
        assert p.is_concave
        assert not p.is_convex


class TestPolygonMethods:
    def test_get_rect(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)]
        p = Polygon(pts)
        r = p.get_rect()
        assert r.w == pytest.approx(10) and r.h == pytest.approx(10)

    def test_translate(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10)]
        p = Polygon(pts)
        p.move(Vec2(5, 5))
        assert p.points[0] == Vec2(5, 5)

    def test_copy(self):
        pts = [Vec2(0, 0), Vec2(10, 0), Vec2(10, 10)]
        p = Polygon(pts)
        c = p.copy()
        c.move(Vec2(100, 100))
        assert p.points[0] == Vec2(0, 0)
