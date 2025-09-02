import pykraken as kn

kn.init()
kn.window.create("Kraken Example", kn.Vec2(320, 240), True)

# paths written to run from the parent directory
booth = kn.PixelArray("example/phone-booth.png")
player = kn.PixelArray("example/idle.png")

booth_mask = kn.Mask(booth, 1)
player_mask = kn.Mask(player, 1)

booth_tex = kn.Texture(booth)
player_tex = kn.Texture(player)

player_rect = player_tex.get_rect()
booth_pos = kn.renderer.get_res() / 2

# kn.time.set_cap(60)

while kn.window.is_open():
    kn.event.poll()
    player_rect.center = kn.mouse.get_pos()

    dir_vec = kn.Vec2(
        kn.key.is_pressed(kn.S_d) - kn.key.is_pressed(kn.S_a),
        kn.key.is_pressed(kn.S_s) - kn.key.is_pressed(kn.S_w)
    )
    dir_vec.normalize()
    booth_pos += dir_vec * 150 * kn.time.get_delta()
    
    kn.renderer.clear("#141414")

    booth_tex.render(booth_pos)
    player_tex.render(player_rect)

    diff_mask = booth_mask.get_overlap_mask(player_mask, booth_pos - player_rect.center)
    if diff_mask.size > kn.Vec2():
        diff_array = diff_mask.get_pixel_array(kn.color.YELLOW)
        diff_tex = kn.Texture(diff_array)
        
        # Calculate the position to render the overlap mask
        # The overlap mask is positioned relative to the booth mask's coordinate system
        offset = booth_pos - player_rect.center
        
        # Find the actual intersection area bounds
        booth_rect = kn.Rect(booth_pos - booth_tex.get_size() / 2, booth_tex.get_size())
        player_world_rect = player_rect
        
        # Calculate intersection rectangle in world coordinates
        intersection_left = max(booth_rect.left, player_world_rect.left)
        intersection_top = max(booth_rect.top, player_world_rect.top)
        intersection_right = min(booth_rect.right, player_world_rect.right)
        intersection_bottom = min(booth_rect.bottom, player_world_rect.bottom)
        
        # Position to render the difference mask
        diff_pos = kn.Vec2(intersection_left, intersection_top)
        diff_tex.render(diff_pos, kn.TOP_LEFT)
    
    kn.renderer.present()

kn.quit()
