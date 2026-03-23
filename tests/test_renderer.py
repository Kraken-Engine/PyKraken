import pykraken
from pykraken import renderer, window, Color


def test_renderer_clear_and_read_pixels():
    pykraken.init()
    try:
        window.create("test", 64, 64, handle_close=False)
        try:
            renderer.clear(Color(10, 20, 30, 255))
            renderer.present()
            pa = renderer.read_pixels()
            c = pa.get_at(0, 0)
            assert (c.r, c.g, c.b) == (10, 20, 30)
        finally:
            window.close()
    finally:
        pykraken.quit()
