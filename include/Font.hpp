#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "Color.hpp"
#include "Math.hpp"

namespace kn
{
class Font
{
  public:
    Font(const std::string& fileDir, int ptSize);
    ~Font();

    void render(const std::string& text, const Vec2& pos = {}, const Color& color = {255, 255, 255},
                uint32_t wrapWidth = 0) const;

  private:
    TTF_Font* m_font = nullptr;
    TTF_Text* m_text = nullptr;
};

void _init();
void _quit();
} // namespace kn
