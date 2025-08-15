import pykraken as kn
# import random

kn.init()
kn.window.create("Kraken Example", (1200, 900))
clock = kn.Clock()

bg_color = kn.Color("#141414")

# rects = [kn.Rect(kn.Vec2(random.randint(0, 1200), random.randint(0, 900)), kn.Vec2(10, 10)) for _ in range(100000)]

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

    kn.draw.polygon([kn.Vec2(600, 100), kn.Vec2(700, 200), kn.Vec2(500, 200)], "#FF0000", True)

    kn.renderer.present()

kn.quit()
