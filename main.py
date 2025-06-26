import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (900, 500))
renderer = kn.Renderer((200, 150))

bg_color = kn.Color("#141414")
texture = kn.Texture(renderer, "img.png")
circle = kn.Circle((0, 0), 21)

while kn.window.is_open():
    kn.event.poll()
    
    if kn.key.is_just_pressed(kn.K_ESC):
        kn.window.close()
    
    renderer.clear(bg_color)
    renderer.draw(texture)
    
    mouse_pos = renderer.to_view(kn.mouse.get_pos())
    
    renderer.draw(((0, 0), mouse_pos), (255, 255, 255))
    renderer.present()

kn.quit()
