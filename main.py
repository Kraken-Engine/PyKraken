import pykraken as kn


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

while kn.window.is_open():
    kn.event.poll()

    kn.renderer.clear("#444")
    astronaut.update()
    kn.renderer.present()

kn.quit()
