import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (900, 500))
renderer = kn.Renderer((200, 150))
clock = kn.Clock()

bg_color = kn.Color("#141414")
texture = kn.Texture(renderer, "img.png")
rect = texture.get_rect()
rect.scale_by(0.5)
pos = kn.Vec2()
texture.flip.h = True

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
        texture.angle -= dt * 30
    if kn.key.is_pressed(kn.S_x):
        texture.angle += dt * 30
        
    rect.center = pos
    
    renderer.clear(bg_color)
    renderer.draw(texture, rect)
    
    renderer.present()

kn.quit()
