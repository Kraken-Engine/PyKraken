import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)

camera = kn.Camera()
camera.set()

sound = kn.Audio("drums.wav")

while kn.window.is_open():
    kn.event.poll()
    if kn.key.is_just_pressed(kn.S_SPACE):
        sound.play(5000)
    if kn.key.is_just_pressed(kn.S_q):
        sound.stop()
    
    kn.renderer.clear("#141414")
    kn.renderer.present()

kn.quit()
