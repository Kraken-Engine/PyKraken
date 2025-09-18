import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (320, 240), True)
kn.window.set_icon("example/phone-booth.png")
kn.time.set_target(240)

tilemap = kn.TileMap("example/room.tmx")

while kn.window.is_open():
    kn.event.poll()
    
    kn.renderer.clear()
    tilemap.render()
    kn.renderer.present()

kn.quit()
