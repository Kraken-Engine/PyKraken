import pykraken as kn
from pykraken import fx


class Astronaut(kn.Sprite):
    def __init__(self):
        super().__init__()

        self.transform.anchor = kn.Anchor.CENTER
        self.transform.pos = kn.renderer.get_res() / 2
        self.transform.size = (19, 30)

        self.anim = kn.AnimationController()
        self.anim.load_sprite_sheet("astroidle.png", (19, 30), [kn.SheetStrip("idle", 10, 8)])

        self.texture = self.anim.texture

    def update(self):
        self.clip = self.anim.clip

        self.draw()


kn.init(debug=True)
kn.window.create("test", (400, 300), scaled=True)

astronaut = Astronaut()

# Set up an animation sequence for the astronaut
orch = kn.Orchestrator(astronaut)
orch.parallel(
    fx.move_to(pos=(100, 150), dur=1.0, ease=kn.ease.out_quad),
    fx.scale_to(scale=2.0, dur=1.0, ease=kn.ease.out_back),
).then(
    fx.shake(amp=5, freq=30, dur=0.3)
).then(
    fx.rotate_to(angle=0.5, dur=0.5, ease=kn.ease.in_out_cubic)
).parallel(
    fx.move_to(pos=(300, 150), dur=1.5, ease=kn.ease.in_out_sin),
    fx.rotate_to(angle=-0.5, dur=1.5, ease=kn.ease.in_out_sin),
).then(
    fx.wait(dur=0.5)
).parallel(
    fx.move_to(pos=(200, 150), dur=0.8, ease=kn.ease.out_bounce),
    fx.scale_to(scale=1.0, dur=0.8, ease=kn.ease.out_elastic),
    fx.rotate_to(angle=0.0, dur=0.8, ease=kn.ease.out_quad),
).then(
    fx.call(lambda: print("Animation complete!"))
)
orch.looping = True
orch.finalize()
orch.play()

while kn.window.is_open():
    kn.event.poll()

    kn.renderer.clear("#444")
    astronaut.update()
    kn.renderer.present()

kn.quit()
