import pykraken as kn
# import random

kn.init()
kn.window.create("Kraken Example", (320, 240), True)
clock = kn.Clock()

bg_color = kn.Color("#141414")

# rects = [kn.Rect(kn.Vec2(random.randint(0, 1200), random.randint(0, 900)), kn.Vec2(10, 10)) for _ in range(100000)]

surf = kn.Surface((20, 20))
surf.fill("#fff")
tex = kn.Texture(surf)

timer = kn.Timer(0.1)
timer.start()

while kn.window.is_open():
    dt = clock.tick()
    kn.event.poll()

    kn.renderer.clear(bg_color)

    # kn.draw.rects(rects, "#FFFFFF")
    
    if timer.done:
        timer.start()
        kn.window.set_title(f"FPS: {clock.get_fps()}")

    # kn.draw.polygon([(100, 100), kn.mouse.get_pos(), (150, 200)], "#FF0000")

    # Draw concave polygon
    kn.draw.polygon([kn.mouse.get_pos(), (150, 50), (200, 100), (150, 150)], "#FF0000", True)

    tex.render(kn.renderer.get_res() / 2, kn.CENTER)

    kn.renderer.present()

kn.quit()
