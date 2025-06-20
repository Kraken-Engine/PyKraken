import pykraken as kn

kn.init()

kn.window.create((200, 150), scaled=True)
clock = kn.Clock()

class Player:
    def __init__(self):
        self.pos = kn.Vec2(kn.window.get_size()) / 2
        self.speed = 100
        
    def update(self, delta: float):
        dir_vec = kn.input.get_direction("up", "right", "down", "left")
        
        self.pos += dir_vec * self.speed * delta
        kn.draw.circle(self.pos, 6, (255, 255, 255))

player_A = Player()
bg_color = kn.Color("#141414")

camera_A = kn.Camera(-20, 20)
camera_A.set()

kn.input.bind(
    "up",
    [
        kn.InputAction(kn.S_w),
        kn.InputAction(kn.S_UP),
        kn.InputAction(kn.C_LY, False, 0),
    ]
)
kn.input.bind(
    "right",
    [
        kn.InputAction(kn.S_d),
        kn.InputAction(kn.S_RIGHT),
        kn.InputAction(kn.C_LX, True, 0),
    ]
)
kn.input.bind(
    "down",
    [
        kn.InputAction(kn.S_s),
        kn.InputAction(kn.S_DOWN),
        kn.InputAction(kn.C_LY, True, 0),
    ]
)
kn.input.bind(
    "left",
    [
        kn.InputAction(kn.S_a),
        kn.InputAction(kn.S_LEFT),
        kn.InputAction(kn.C_LX, False, 0),
    ]
)

while kn.window.is_open():
    delta = clock.tick(240)
    kn.event.poll()
    
    kn.window.clear(bg_color)
    player_A.update(delta)
    # playerB.update(delta)
    
    kn.window.flip()

kn.quit()
