import pytest

from pykraken import ease


ALL_EASING_FUNCS = [
    ease.linear,
    ease.in_quad,
    ease.out_quad,
    ease.in_out_quad,
    ease.in_cubic,
    ease.out_cubic,
    ease.in_out_cubic,
    ease.in_quart,
    ease.out_quart,
    ease.in_out_quart,
    ease.in_quint,
    ease.out_quint,
    ease.in_out_quint,
    ease.in_sin,
    ease.out_sin,
    ease.in_out_sin,
    ease.in_circ,
    ease.out_circ,
    ease.in_out_circ,
    ease.in_expo,
    ease.out_expo,
    ease.in_out_expo,
    ease.in_elastic,
    ease.out_elastic,
    ease.in_out_elastic,
    ease.in_back,
    ease.out_back,
    ease.in_out_back,
    ease.in_bounce,
    ease.out_bounce,
    ease.in_out_bounce,
]


@pytest.mark.parametrize("func", ALL_EASING_FUNCS, ids=lambda f: f.__name__)
class TestEasingBoundaries:
    def test_start_is_zero(self, func):
        assert func(0.0) == pytest.approx(0.0, abs=1e-4)

    def test_end_is_one(self, func):
        assert func(1.0) == pytest.approx(1.0, abs=1e-2)

    def test_midpoint_is_finite(self, func):
        result = func(0.5)
        assert result == result  # not NaN


class TestLinear:
    def test_quarter(self):
        assert ease.linear(0.25) == pytest.approx(0.25)

    def test_half(self):
        assert ease.linear(0.5) == pytest.approx(0.5)

    def test_three_quarter(self):
        assert ease.linear(0.75) == pytest.approx(0.75)


class TestEaseInOut:
    def test_in_out_quad_symmetry(self):
        """in_out functions should be < 0.5 at t < 0.5 and > 0.5 at t > 0.5."""
        assert ease.in_out_quad(0.25) < 0.5
        assert ease.in_out_quad(0.75) > 0.5

    def test_in_out_cubic_symmetry(self):
        assert ease.in_out_cubic(0.25) < 0.5
        assert ease.in_out_cubic(0.75) > 0.5
