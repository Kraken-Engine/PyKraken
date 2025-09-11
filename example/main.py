import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)

anim_controller = kn.AnimationController()
anim_controller.load_sprite_sheet("walk", "example/walk.png", (21, 41), 7)
anim_controller.load_sprite_sheet("idle", "example/idle.png", (21, 41), 7)
anim_rect = kn.Rect()
anim_rect.size = (21, 41)
anim_rect.center = kn.renderer.get_res() / 2

kn.time.set_target(240)

while kn.window.is_open():
    kn.event.poll()
    anim_controller.set("walk" if kn.key.is_pressed(kn.S_w) else "idle")
    
    kn.renderer.clear()
    
    curr_frame = anim_controller.current_frame
    curr_frame.tex.flip.h = True
    curr_frame.tex.render(anim_rect, curr_frame.rect)
    
    kn.renderer.present()

kn.quit()
