import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)
# kn.window.set_icon("example/phone-booth.png")
kn.time.set_target(60)

tilemap = kn.TileMap("example/room.tmx")

player_anim = kn.AnimationController()
player_anim.load_sprite_sheet("idle", "example/idle.png", (21, 41), 7)
player_rect = kn.Rect(160.5, 120, 21.4, 41)
camera = kn.Camera()
camera.set()

while kn.window.is_open():
    for event in kn.event.poll():
        if event.type == kn.MOUSE_WHEEL:
            kn.time.set_scale(max(0.1, kn.time.get_scale() + event.y * 0.1))

    dir_vec = kn.Vec2(
        kn.key.is_pressed(kn.S_d) - kn.key.is_pressed(kn.S_a),
        kn.key.is_pressed(kn.S_s) - kn.key.is_pressed(kn.S_w)
    )
    dir_vec.normalize()
    vel = kn.time.get_delta() * 50 * dir_vec
    player_rect.center += vel
    camera.pos += vel

    frame = player_anim.current_frame

    kn.renderer.clear()
    tilemap.render()
    kn.renderer.draw(frame.tex, player_rect, frame.src)
    kn.renderer.present()

kn.quit()
