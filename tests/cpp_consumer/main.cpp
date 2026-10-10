#include <array>
#include <kraken/KrakenEngine.hpp>

int main()
{
    kn::init();
    kn::window::create("Kraken SDK smoke test", 640, 360);
    kn::event::poll();
    kn::renderer::clear(kn::Color::BLACK);
    {
        kn::PixelArray pixels(2, 2);
        pixels.fill(kn::Color::WHITE);
        kn::Texture texture(pixels);
        kn::renderer::Batcher batcher;
        batcher.preallocate(2);
        const std::array<double, 8> sprites = {10, 10, 0, 16, 50, 10, 0, 16};
        kn::renderer::drawBatch(texture, sprites, 4, &batcher);
        kn::renderer::present();
        batcher.free();
    }
    kn::quit();
    return 0;
}
