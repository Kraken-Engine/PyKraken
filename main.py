import pykraken as kn

kn.init()
kn.window.create("test", (300, 300), scaled=True)

mouse_rect = kn.Rect(0, 0, 8, 8)

while kn.window.is_open():
    for event in kn.event.poll():
        if event.type == kn.KEY_DOWN:
            if event.key == kn.K_ESC:
                kn.window.close()
            elif event.key == kn.K_F11:
                kn.window.set_fullscreen(not kn.window.is_fullscreen())

    kn.renderer.clear("#444")

    mouse_rect.center = kn.mouse.get_pos()
    kn.draw.rect(mouse_rect, color="#fff")

    kn.renderer.present()

kn.quit()
