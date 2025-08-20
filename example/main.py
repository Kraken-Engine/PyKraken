import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)

while kn.window.is_open():
    kn.event.poll()
    
    kn.renderer.clear("#141414")
    kn.renderer.present()

kn.quit()
