import pykraken as kn
# import random

kn.init()
kn.window.create("Kraken Example", (320, 240), True)
clock = kn.Clock()

bg_color = kn.Color("#141414")

# points = [kn.Vec2(random.randint(0, 1200), random.randint(0, 900)) for _ in range(400000)]

# rect1 = kn.Rect(100, 100, 20, 20)
# rect2 = rect1.copy()

timer = kn.Timer(0.1)
timer.start()

color = kn.Color("#FFFFFF")

while kn.window.is_open():
    clock.tick()
    kn.event.poll()

    kn.renderer.clear(bg_color)

    # kn.draw.points(points, color)

    if timer.done:
        timer.start()
        kn.window.set_title(f"FPS: {clock.get_fps()}")

    # Concave polygon
    kn.draw.polygon([kn.mouse.get_pos(), (150, 50), (200, 100), (150, 150)], "#FF0000", True)

    # tex.render(kn.renderer.get_res() / 2, kn.CENTER)

    kn.renderer.present()

kn.quit()
