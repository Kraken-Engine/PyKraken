import pytest

from pykraken import Vec2, Rect, Circle, Line, Polygon, collision


class TestOverlapRectRect:
    def test_overlapping(self):
        assert collision.overlap(Rect(0, 0, 10, 10), Rect(5, 5, 10, 10))

    def test_not_overlapping(self):
        assert not collision.overlap(Rect(0, 0, 10, 10), Rect(20, 20, 10, 10))

    def test_touching_edge(self):
        # Touching at edge — implementation-dependent, just check it doesn't crash
        collision.overlap(Rect(0, 0, 10, 10), Rect(10, 0, 10, 10))


class TestOverlapRectCircle:
    def test_overlapping(self):
        assert collision.overlap(Rect(0, 0, 10, 10), Circle(Vec2(5, 5), 3))

    def test_not_overlapping(self):
        assert not collision.overlap(Rect(0, 0, 10, 10), Circle(Vec2(50, 50), 1))


class TestOverlapRectPoint:
    def test_inside(self):
        assert collision.overlap(Rect(0, 0, 10, 10), Vec2(5, 5))

    def test_outside(self):
        assert not collision.overlap(Rect(0, 0, 10, 10), Vec2(20, 20))


class TestOverlapRectLine:
    def test_intersecting(self):
        assert collision.overlap(Rect(0, 0, 10, 10), Line(Vec2(-5, 5), Vec2(15, 5)))

    def test_not_intersecting(self):
        assert not collision.overlap(Rect(0, 0, 10, 10), Line(Vec2(20, 20), Vec2(30, 30)))


class TestOverlapCircleCircle:
    def test_overlapping(self):
        assert collision.overlap(Circle(Vec2(0, 0), 5), Circle(Vec2(3, 0), 5))

    def test_not_overlapping(self):
        assert not collision.overlap(Circle(Vec2(0, 0), 1), Circle(Vec2(100, 0), 1))


class TestOverlapCirclePoint:
    def test_inside(self):
        assert collision.overlap(Circle(Vec2(0, 0), 5), Vec2(1, 1))

    def test_outside(self):
        assert not collision.overlap(Circle(Vec2(0, 0), 1), Vec2(10, 10))


class TestOverlapCircleLine:
    def test_intersecting(self):
        assert collision.overlap(Circle(Vec2(5, 5), 3), Line(Vec2(0, 5), Vec2(10, 5)))

    def test_not_intersecting(self):
        assert not collision.overlap(Circle(Vec2(5, 5), 1), Line(Vec2(20, 20), Vec2(30, 30)))


class TestContains:
    def test_rect_contains_rect(self):
        assert collision.contains(Rect(0, 0, 100, 100), Rect(10, 10, 20, 20))

    def test_rect_does_not_contain_rect(self):
        assert not collision.contains(Rect(0, 0, 10, 10), Rect(5, 5, 20, 20))

    def test_rect_contains_circle(self):
        assert collision.contains(Rect(0, 0, 100, 100), Circle(Vec2(50, 50), 10))

    def test_rect_does_not_contain_circle(self):
        assert not collision.contains(Rect(0, 0, 10, 10), Circle(Vec2(5, 5), 20))

    def test_circle_contains_circle(self):
        assert collision.contains(Circle(Vec2(0, 0), 10), Circle(Vec2(0, 0), 5))

    def test_circle_does_not_contain_circle(self):
        assert not collision.contains(Circle(Vec2(0, 0), 5), Circle(Vec2(0, 0), 10))


class TestOverlapPolygon:
    def test_polygon_rect_overlap(self):
        poly = Polygon([Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)])
        assert collision.overlap(poly, Rect(5, 5, 10, 10))

    def test_polygon_rect_no_overlap(self):
        poly = Polygon([Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)])
        assert not collision.overlap(poly, Rect(50, 50, 10, 10))

    def test_polygon_point_overlap(self):
        poly = Polygon([Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)])
        assert collision.overlap(poly, Vec2(5, 5))

    def test_polygon_point_no_overlap(self):
        poly = Polygon([Vec2(0, 0), Vec2(10, 0), Vec2(10, 10), Vec2(0, 10)])
        assert not collision.overlap(poly, Vec2(50, 50))
