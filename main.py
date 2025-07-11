import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (900, 500))
renderer = kn.Renderer((200, 150))
clock = kn.Clock()

bg_color = kn.Color("#141414")

surface = kn.Surface("kn112.png")
surface_gray = kn.transform.grayscale(surface)
surface_inv = kn.transform.invert(surface)

texture_gray = kn.Texture(renderer, surface_gray)
texture_inv = kn.Texture(renderer, surface_inv)

rect = surface.rect
rect.scale_by(0.5)
pos = kn.Vec2()

while kn.window.is_open():
    dt = clock.tick()
    kn.event.poll()
    
    if kn.key.is_just_pressed(kn.K_ESC):
        kn.window.close()
        
    if kn.key.is_pressed(kn.S_a):
        pos.x -= dt * 100
    if kn.key.is_pressed(kn.S_d):
        pos.x += dt * 100
    if kn.key.is_pressed(kn.S_w):
        pos.y -= dt * 100
    if kn.key.is_pressed(kn.S_s):
        pos.y += dt * 100
    if kn.key.is_pressed(kn.S_z):
        texture_gray.angle -= dt * 30
        texture_inv.angle -= dt * 30
    if kn.key.is_pressed(kn.S_x):
        texture_gray.angle += dt * 30
        texture_inv.angle += dt * 30
        
    renderer.clear(bg_color)
    
    rect.center = pos
    renderer.draw(texture_gray, rect)
    rect.center = pos + (30, 30)
    renderer.draw(texture_inv, rect)
    
    renderer.present()

kn.quit()
