#include <kraken/KrakenEngine.hpp>

int main()
{
    kn::init();
    kn::window::create("Kraken SDK smoke test", 640, 360);
    kn::event::poll();
    kn::renderer::clear(kn::Color::BLACK);
    kn::renderer::present();
    kn::quit();
    return 0;
}
