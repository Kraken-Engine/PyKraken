import math

import pytest
import pykraken as kn


@pytest.fixture
def canvas():
    kn.init()
    kn.renderer.set_render_backend(kn.RenderBackend.LEGACY)
    kn.window.create("Draw options tests", 64, 32, handle_close=False)
    target = kn.Texture(64, 32, filter=kn.FilterMode.NEAREST)
    kn.renderer.set_target(target)
    kn.renderer.clear(kn.Color.BLACK)
    yield target
    kn.renderer.set_target()
    del target
    kn.window.close()
    kn.quit()
    kn.renderer.set_render_backend(kn.RenderBackend.AUTO)


def texture(colors):
    pixels = kn.PixelArray(len(colors[0]), len(colors))
    for y, row in enumerate(colors):
        for x, color in enumerate(row):
            pixels.set_at(x, y, color)
    return kn.Texture(pixels, filter=kn.FilterMode.NEAREST)


def rgb(x, y):
    color = kn.renderer.read_pixels().get_at(x, y)
    return color.r, color.g, color.b


def test_source_and_flips_do_not_leak(canvas):
    atlas = texture([[kn.Color.RED, kn.Color.GREEN], [kn.Color.BLUE, kn.Color.WHITE]])
    kn.renderer.draw(atlas, dst=kn.Rect(0, 0, 8, 8), source=kn.Rect(1, 0, 1, 1))
    kn.renderer.draw(atlas, dst=kn.Rect(8, 0, 8, 8), flip_h=True, flip_v=True)
    kn.renderer.draw(atlas, dst=kn.Rect(16, 0, 8, 8))
    assert rgb(2, 2) == (0, 255, 0)
    assert rgb(9, 1) == (255, 255, 255)
    assert rgb(17, 1) == (255, 0, 0)


def test_tint_alpha_and_blend_are_local(canvas):
    white = texture([[kn.Color.WHITE]])
    kn.renderer.draw(white, dst=kn.Rect(0, 0, 4, 4), tint=kn.Color(255, 0, 0, 128), alpha=0.5)
    kn.renderer.draw(white, dst=kn.Rect(4, 0, 4, 4), tint=kn.Color.BLUE)
    kn.renderer.draw(white, dst=kn.Rect(4, 0, 4, 4), tint=kn.Color.RED,
                     alpha=0.5, blend=kn.BlendMode.ADDITIVE)
    kn.renderer.draw(white, dst=kn.Rect(8, 0, 4, 4))
    assert rgb(1, 1) == pytest.approx((64, 0, 0), abs=2)
    assert rgb(5, 1) == pytest.approx((128, 0, 255), abs=2)
    assert rgb(9, 1) == (255, 255, 255)


def test_reusable_options_and_keyword_overrides(canvas):
    white = texture([[kn.Color.WHITE]])
    style = kn.DrawOptions(tint=kn.Color.RED, alpha=0.5, flip_h=True)
    kn.renderer.draw(white, kn.Transform(pos=kn.Vec2(0, 0), scale=4), options=style)
    kn.renderer.draw(white, dst=kn.Rect(4, 0, 4, 4), options=style,
                     tint=kn.Color.BLUE, alpha=1, flip_h=False)
    assert rgb(1, 1) == pytest.approx((128, 0, 0), abs=2)
    assert rgb(5, 1) == (0, 0, 255)
    assert style.tint == kn.Color.RED
    assert style.alpha == 0.5 and style.flip_h


@pytest.mark.parametrize("numpy_batch", [False, True])
def test_per_instance_options(canvas, numpy_batch):
    atlas = texture([[kn.Color.RED, kn.Color.WHITE]])
    transforms = [kn.Transform(pos=kn.Vec2(0, 0), scale=4),
                  kn.Transform(pos=kn.Vec2(8, 0), scale=4)]
    if numpy_batch:
        np = pytest.importorskip("numpy")
        transforms = np.array([[0, 0, 0, 4], [8, 0, 0, 4]], dtype=np.float64)
    styles = [kn.DrawOptions(source=kn.Rect(0, 0, 1, 1)),
              kn.DrawOptions(source=kn.Rect(1, 0, 1, 1), tint=kn.Color.BLUE, alpha=0.5)]
    kn.renderer.draw_batch(atlas, transforms, instance_options=styles)
    kn.renderer.draw(atlas, dst=kn.Rect(16, 0, 8, 4))
    assert rgb(1, 1) == (255, 0, 0)
    assert rgb(9, 1) == pytest.approx((0, 0, 128), abs=2)
    assert rgb(21, 1) == (255, 255, 255)


def test_numpy_geometry_and_array_sources(canvas):
    np = pytest.importorskip("numpy")
    atlas = texture([[kn.Color.RED, kn.Color.WHITE]])
    batcher = kn.renderer.Batcher()
    batcher.preallocate(2)
    kn.renderer.draw(atlas, dst=kn.Rect(20, 0, 4, 4), tint=kn.Color.RED, alpha=0.25)
    rows = np.array([[0, 0, 0, 4, 4, 1, 0, 1, 1],
                     [8, 0, 0, 4, 4, 0, 0, 1, 1]], dtype=np.float64)
    kn.renderer.draw_batch(atlas, rows, batcher=batcher, tint=kn.Color.BLUE, alpha=0.5)
    assert rgb(1, 1) == pytest.approx((0, 0, 128), abs=2)
    assert rgb(9, 1) == (0, 0, 0)
    kn.renderer.draw(atlas, dst=kn.Rect(16, 0, 8, 4))
    assert rgb(21, 1) == (255, 255, 255)


def test_numpy_shared_source_flip_and_tint_alpha(canvas):
    np = pytest.importorskip("numpy")
    atlas = texture([[kn.Color.RED, kn.Color.WHITE]])
    rows = np.array([[0, 0, 0, 4]], dtype=np.float64)
    kn.renderer.draw_batch(atlas, rows, flip_h=True, tint=kn.Color(255, 255, 255, 128), alpha=0.5)
    assert rgb(1, 1) == pytest.approx((64, 64, 64), abs=2)
    assert rgb(5, 1) == pytest.approx((64, 0, 0), abs=2)


def test_nine_slice_source_and_whole_image_flip(canvas):
    atlas = texture([[kn.Color.BLACK, kn.Color.RED, kn.Color.GREEN, kn.Color.BLUE, kn.Color.BLACK],
                     [kn.Color.BLACK, kn.Color.WHITE, kn.Color.WHITE, kn.Color.WHITE, kn.Color.BLACK],
                     [kn.Color.BLACK, kn.Color.BLUE, kn.Color.GREEN, kn.Color.RED, kn.Color.BLACK]])
    kn.renderer.draw_9slice(atlas, kn.Rect(0, 0, 9, 9), kn.Rect(1, 1, 1, 1),
                           source=kn.Rect(1, 0, 3, 3), flip_h=True)
    assert rgb(0, 0) == (0, 0, 255)
    assert rgb(8, 0) == (255, 0, 0)
    assert rgb(4, 4) == (255, 255, 255)


@pytest.mark.parametrize("alpha", [-1, 2, math.nan, math.inf])
def test_invalid_alpha(canvas, alpha):
    tex = texture([[kn.Color.WHITE]])
    with pytest.raises(ValueError):
        kn.renderer.draw(tex, alpha=alpha)


def test_batch_length_validation(canvas):
    tex = texture([[kn.Color.WHITE]])
    with pytest.raises(ValueError):
        kn.renderer.draw_batch(tex, [kn.Transform()], instance_options=[])
    with pytest.raises(ValueError):
        kn.renderer.draw_batch(tex, [kn.Transform()], clip_rects=[])


def test_source_validation_and_zero_area(canvas):
    tex = texture([[kn.Color.WHITE]])
    with pytest.raises(ValueError):
        kn.renderer.draw(tex, source=kn.Rect(0, 0, 2, 1))
    kn.renderer.draw(tex, source=kn.Rect(0, 0, 0, 0))
    assert rgb(0, 0) == (0, 0, 0)


def test_texture_has_no_render_properties(canvas):
    tex = texture([[kn.Color.WHITE]])
    for name in ("clip_area", "tint", "alpha", "flip", "make_additive", "make_multiply", "make_normal"):
        assert not hasattr(tex, name)


def test_native_nine_slice_modulation_is_local(canvas):
    atlas = texture([[kn.Color.WHITE] * 3 for _ in range(3)])
    kn.renderer.draw_9slice(atlas, kn.Rect(0, 0, 8, 8), kn.Rect(1, 1, 1, 1),
                           tint=kn.Color.BLUE, alpha=0.5)
    kn.renderer.draw_9slice(atlas, kn.Rect(8, 0, 8, 8), kn.Rect(1, 1, 1, 1))
    assert rgb(4, 4) == pytest.approx((0, 0, 128), abs=2)
    assert rgb(12, 4) == (255, 255, 255)


def test_tilemap_does_not_leak_source_or_opacity(canvas, tmp_path):
    import struct
    import zlib

    def chunk(kind, data):
        return (struct.pack(">I", len(data)) + kind + data +
                struct.pack(">I", zlib.crc32(kind + data)))

    image = (b"\x89PNG\r\n\x1a\n" +
             chunk(b"IHDR", struct.pack(">IIBBBBB", 2, 1, 8, 2, 0, 0, 0)) +
             chunk(b"IDAT", zlib.compress(bytes([0, 255, 0, 0, 255, 255, 255]))) +
             chunk(b"IEND", b""))
    (tmp_path / "atlas.png").write_bytes(image)
    path = tmp_path / "map.tmx"
    path.write_text('''<?xml version="1.0" encoding="UTF-8"?>
<map version="1.10" tiledversion="1.10.2" orientation="orthogonal" renderorder="right-down"
 width="2" height="1" tilewidth="1" tileheight="1" infinite="0">
 <tileset firstgid="1" name="atlas" tilewidth="1" tileheight="1" tilecount="2" columns="2">
  <image source="atlas.png" width="2" height="1"/>
 </tileset>
 <layer id="1" name="tiles" width="2" height="1" opacity="0.5">
  <data encoding="csv">2,1</data>
 </layer>
</map>''')
    tilemap = kn.tilemap.Map(path)
    tilemap.draw()
    assert rgb(0, 0) == pytest.approx((128, 128, 128), abs=2)
    shared = tilemap.tile_sets[0].texture
    kn.renderer.draw(shared, dst=kn.Rect(16, 0, 8, 4))
    assert rgb(16, 1) == (255, 0, 0)
    assert rgb(23, 1) == (255, 255, 255)


def test_numpy_blend_runs_preserve_order(canvas):
    np = pytest.importorskip("numpy")
    tex = texture([[kn.Color.WHITE]])
    styles = [kn.DrawOptions(tint=kn.Color.RED),
              kn.DrawOptions(tint=kn.Color.BLUE, alpha=0.5, blend=kn.BlendMode.ADDITIVE),
              kn.DrawOptions(tint=kn.Color.GREEN, alpha=0.5),
              kn.DrawOptions(tint=kn.Color(128, 255, 255), blend=kn.BlendMode.MULTIPLY)]
    transforms = [kn.Transform(pos=kn.Vec2(0, 0), scale=8) for _ in styles]
    rows = np.array([[32, 0, 0, 8] for _ in styles], dtype=np.float64)
    kn.renderer.draw_batch(tex, transforms, instance_options=styles)
    batcher = kn.renderer.Batcher()
    batcher.preallocate(len(styles))
    kn.renderer.draw_batch(tex, rows, batcher=batcher, instance_options=styles)
    assert rgb(33, 1) == pytest.approx(rgb(1, 1), abs=2)


def test_numpy_clips_and_buffer_reuse(canvas):
    np = pytest.importorskip("numpy")
    atlas = texture([[kn.Color.RED, kn.Color.WHITE]])
    batcher = kn.renderer.Batcher()
    batcher.preallocate(3)
    rows = np.array([[0, 0, 0, 4, 4, 0, 0, 1, 1],
                     [8, 0, 0, 4, 4, 0, 0, 1, 1]], dtype=np.float64)
    kn.renderer.draw_batch(atlas, rows, batcher=batcher,
                           clip_rects=[kn.Rect(1, 0, 1, 1), kn.Rect(0, 0, 1, 1)])
    assert rgb(1, 1) == (255, 255, 255)
    assert rgb(9, 1) == (255, 0, 0)
    kn.renderer.clear(kn.Color.BLACK)
    smaller = np.array([[16, 0, 0, 4]], dtype=np.float64)
    kn.renderer.draw_batch(atlas, smaller, batcher=batcher)
    assert rgb(1, 1) == (0, 0, 0)
    assert rgb(17, 1) == (255, 0, 0)
    kn.renderer.clear(kn.Color.BLACK)
    kn.renderer.draw_batch(atlas, np.empty((0, 2), dtype=np.float64), batcher=batcher)
    assert rgb(17, 1) == (0, 0, 0)
    batcher.free()
    kn.renderer.draw_batch(atlas, smaller, batcher=batcher)
    assert rgb(17, 1) == (255, 0, 0)


@pytest.mark.parametrize("columns", [0, 1, 6, 7, 8, 10])
def test_numpy_rejects_invalid_row_layout(canvas, columns):
    np = pytest.importorskip("numpy")
    tex = texture([[kn.Color.WHITE]])
    with pytest.raises(ValueError):
        kn.renderer.draw_batch(tex, np.zeros((0, columns), dtype=np.float64))
