import pykraken as kn

kn.init()
kn.window.create("Kraken Example", (1200, 800), True)
kn.time.set_target(60)

reg = kn.Font("kraken-clean", 48)
ital = kn.Font("kraken-clean", 48)
ital.set_italic(True)
bold = kn.Font("kraken-clean", 48)
bold.set_bold(True)
under = kn.Font("kraken-clean", 48)
under.set_underline(True)
strike = kn.Font("kraken-clean", 48)
strike.set_strikethrough(True)
all_of = kn.Font("kraken-clean", 48)
all_of.set_italic(True)
all_of.set_bold(True)
all_of.set_underline(True)
all_of.set_strikethrough(True)

while kn.window.is_open():
    kn.event.poll()

    kn.renderer.clear()
    reg.draw("This is Kraken Engine", color=(255, 255, 255))
    ital.draw("This is Kraken Engine", color=(255, 255, 255), pos=(0, 48))
    bold.draw("This is Kraken Engine", color=(255, 255, 255), pos=(0, 48 * 2))
    under.draw("This is Kraken Engine", color=(255, 255, 255), pos=(0, 48 * 3))
    strike.draw("This is Kraken Engine", color=(255, 255, 255), pos=(0, 48 * 4))
    all_of.draw("This is Kraken Engine", color=(255, 255, 255), pos=(0, 48 * 5))
    kn.renderer.present()

kn.quit()
