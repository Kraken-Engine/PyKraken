import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)

booth = kn.PixelArray("phone-booth.png")
player = kn.PixelArray("idle.png")

booth_mask = kn.Mask(booth, 1)
player_mask = kn.Mask(player, 1)

booth_tex = kn.Texture(booth)
player_tex = kn.Texture(player)

booth_pos = kn.renderer.get_res() / 2

kn.time.set_cap(60)

while kn.window.is_open():
    kn.event.poll()
    player_pos = kn.mouse.get_pos()

    dir_vec = kn.Vec2(
        kn.key.is_pressed(kn.S_d) - kn.key.is_pressed(kn.S_a),
        kn.key.is_pressed(kn.S_s) - kn.key.is_pressed(kn.S_w)
    )
    dir_vec.normalize()
    booth_pos += dir_vec * 150 * kn.time.get_delta()
    
    kn.renderer.clear("#141414")

    booth_tex.render(booth_pos, kn.CENTER)
    player_tex.render(player_pos, kn.CENTER)

    diff_mask = booth_mask.get_overlap_mask(player_mask, booth_pos - player_pos)
    if diff_mask.size > kn.Vec2():
        diff_array = diff_mask.get_pixelarray(kn.color.YELLOW)
        diff_tex = kn.Texture(diff_array)
        diff_tex.render(booth_pos, kn.CENTER)
    
    kn.renderer.present()

kn.quit()
