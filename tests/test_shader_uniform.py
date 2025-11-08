import struct
import pytest

from pykraken.shader_uniform import ShaderUniform


class MyUniform(ShaderUniform):
    x: float
    y: int
    f2: tuple[float, float]
    flag: bool


def test_to_bytes_basic():
    u = MyUniform(x=1.5, y=2, f2=(0.5, 0.25), flag=True)
    b = u.to_bytes()
    # packing format should be: float, int, 2 floats, bool
    fmt = "fi2f?"
    # use unpack_from so extra padding bytes (native alignment) don't prevent unpacking
    vals = struct.unpack_from(fmt, b, 0)
    assert pytest.approx(vals[0], rel=1e-6) == 1.5
    assert vals[1] == 2
    assert pytest.approx(vals[2], rel=1e-6) == 0.5
    assert pytest.approx(vals[3], rel=1e-6) == 0.25
    assert vals[4] is True


def test_invalid_length_raises():
    class Bad(ShaderUniform):
        v: tuple[float, float, float, float, float]

    with pytest.raises(ValueError):
        Bad(v=(1, 2, 3, 4, 5)).to_bytes()


def test_unsupported_type_raises():
    class Bad2(ShaderUniform):
        s: str

    with pytest.raises(TypeError):
        Bad2(s="no").to_bytes()
