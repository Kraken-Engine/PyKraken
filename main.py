import pykraken as kn

kn.init()
kn.window.create("Kraken Example", True)
renderer = kn.Renderer((200, 150))

bg_color = kn.Color("#141414")
texture = kn.Texture(renderer, "img.png")

while kn.window.is_open():
    kn.event.poll()
    
    renderer.clear(bg_color)
    renderer.draw(texture)
    renderer.present()

kn.quit()
