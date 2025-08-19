import pykraken as kn
import random
import numpy as np

kn.init()
kn.window.create("Kraken Example", (320, 240), True)

points_array = np.array(
    [kn.Vec2(random.randint(0, 1200), random.randint(0, 900)) for _ in range(100000)],
    dtype=np.float64
)

timer = kn.Timer(0.1)
timer.start()

camera = kn.Camera()
camera.set()

while kn.window.is_open():
    kn.event.poll()
    if timer.done:
        timer.start()
        kn.window.set_title(f"FPS: {kn.time.get_fps():.2f}")

    vec = kn.Vec2(
        kn.key.is_pressed(kn.S_d) - kn.key.is_pressed(kn.S_a),
        kn.key.is_pressed(kn.S_s) - kn.key.is_pressed(kn.S_w),
    )
    vec.normalize()
    camera.pos += vec
        
    kn.renderer.clear("#141414")

    kn.draw.points_from_ndarray(points_array, "#FFF")

    kn.renderer.present()

kn.quit()
