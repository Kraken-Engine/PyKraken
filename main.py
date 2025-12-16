import pykraken as kn

kn.init(debug=True)
kn.window.create("test", (1920, 1080), scaled=True)

anim = kn.AnimationController()
anim.load_sprite_sheet("astroidle.png", (19, 30), [kn.SheetStrip("idle", 10, 6)])
# anim.looping = False

dst = kn.Rect(0, 0, 19, 30)
dst.center = kn.renderer.get_res() / 2

font = kn.Font("kraken-retro", 8)
text = kn.Text(font)
text.text = "Hello, PyKraken!"

while kn.window.is_open():
    for event in kn.event.poll():
        if event.type == kn.KEY_DOWN:
            if event.key == kn.K_ESC:
                kn.window.close()
            elif event.key == kn.K_F11:
                kn.window.set_fullscreen(not kn.window.is_fullscreen())

    kn.renderer.clear("#444")

    frame = anim.current_frame
    kn.renderer.draw(frame.tex, dst, frame.src)

    text.draw()

    kn.renderer.present()

kn.quit()
