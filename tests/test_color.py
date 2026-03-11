import pytest

from pykraken import Color, color


class TestColorConstruction:
    def test_default(self):
        c = Color()
        assert (c.r, c.g, c.b, c.a) == (0, 0, 0, 255)

    def test_rgb(self):
        c = Color(100, 150, 200)
        assert (c.r, c.g, c.b, c.a) == (100, 150, 200, 255)

    def test_rgba(self):
        c = Color(100, 150, 200, 128)
        assert (c.r, c.g, c.b, c.a) == (100, 150, 200, 128)

    def test_from_hex_string(self):
        c = Color("#FF0000")
        assert c.r == 255 and c.g == 0 and c.b == 0


class TestColorProperties:
    def test_hex_roundtrip(self):
        c = Color(255, 128, 0)
        h = c.hex  # returns "#FF8000FF" (RRGGBBAA)
        c2 = Color(h)
        assert c2.r == c.r and c2.g == c.g and c2.b == c.b and c2.a == c.a

    def test_set_hex(self):
        c = Color()
        c.hex = "#00FF00"
        assert c.g == 255


class TestColorStaticConstants:
    def test_named_colors(self):
        assert Color.WHITE == Color(255, 255, 255)
        assert Color.BLACK == Color(0, 0, 0)
        assert Color.RED == Color(255, 0, 0)
        assert Color.GREEN == Color(0, 255, 0)
        assert Color.BLUE == Color(0, 0, 255)

    def test_gray_grey_alias(self):
        assert Color.GRAY == Color.GREY


class TestColorOperators:
    def test_equality(self):
        assert Color(1, 2, 3) == Color(1, 2, 3)
        assert Color(1, 2, 3) != Color(1, 2, 4)

    def test_mul_scalar(self):
        c = Color(100, 100, 100) * 0.5
        assert c.r == 50 and c.g == 50 and c.b == 50

    def test_div_scalar(self):
        c = Color(100, 200, 50) / 2.0
        assert c.r == 50 and c.g == 100 and c.b == 25

    def test_indexing(self):
        c = Color(10, 20, 30, 40)
        assert c[0] == 10
        assert c[1] == 20
        assert c[2] == 30
        assert c[3] == 40

    def test_len(self):
        assert len(Color()) == 4

    def test_iter(self):
        assert list(Color(10, 20, 30, 40)) == [10, 20, 30, 40]

    def test_copy(self):
        c = Color(1, 2, 3, 4)
        c2 = c.copy()
        assert c == c2
        c2.r = 99
        assert c.r == 1


class TestColorSubmodule:
    def test_from_hex(self):
        c = color.from_hex("#FF8000")
        assert c.r == 255 and c.g == 128 and c.b == 0

    def test_lerp(self):
        c = color.lerp(Color.BLACK, Color.WHITE, 0.5)
        assert c.r == pytest.approx(127, abs=1)
        assert c.g == pytest.approx(127, abs=1)
        assert c.b == pytest.approx(127, abs=1)

    def test_lerp_boundaries(self):
        assert color.lerp(Color.BLACK, Color.WHITE, 0.0) == Color.BLACK
        assert color.lerp(Color.BLACK, Color.WHITE, 1.0) == Color.WHITE

    def test_invert(self):
        c = color.invert(Color(255, 0, 128))
        assert c.r == 0 and c.g == 255 and c.b == 127

    def test_grayscale(self):
        c = color.grayscale(Color(255, 0, 0))
        assert c.r == c.g == c.b
