#include "Font.hpp"
#include "Renderer.hpp"
#include "misc/SpaceGrotesk.h"
#include "misc/minecraftia.h"

#include <stdexcept>

namespace kn
{
static TTF_TextEngine* _textEngine = nullptr;

Font::Font(const std::string& fileDir, int ptSize)
{
    if (ptSize < 8)
        ptSize = 8;

    if (fileDir == "kraken-clean")
    {
        SDL_IOStream* rw = SDL_IOFromMem(SpaceGrotesk_ttf, SpaceGrotesk_ttf_len);
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSize));
    }
    else if (fileDir == "kraken-retro")
    {
        SDL_IOStream* rw = SDL_IOFromMem(Minecraftia_Regular_ttf, Minecraftia_Regular_ttf_len);
        const int ptSizeFixed = (ptSize + 4) / 8 * 8; // Round to the nearest multiple of 8
        m_font = TTF_OpenFontIO(rw, true, static_cast<float>(ptSizeFixed));
    }
    else
    {
        m_font = TTF_OpenFont(fileDir.c_str(), static_cast<float>(ptSize));
    }

    if (!m_font)
        throw std::runtime_error("Failed to load font: " + std::string(SDL_GetError()));

    m_text = TTF_CreateText(_textEngine, m_font, "", 0);
}

Font::~Font()
{
    if (m_text)
        TTF_DestroyText(m_text);
    m_text = nullptr;

    if (m_font)
        TTF_CloseFont(m_font);
    m_font = nullptr;
}

void Font::render(const std::string& text, const Vec2& pos, const Color& color, int wrapWidth) const
{
    if (wrapWidth < 0)
        wrapWidth = 0;

    TTF_SetTextString(m_text, text.c_str(), 0);
    if (color != WHITE)
        TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
    if (wrapWidth > 0)
        TTF_SetTextWrapWidth(m_text, wrapWidth);

    const auto x = static_cast<int>(std::round(pos.x));
    const auto y = static_cast<int>(std::round(pos.y));
    TTF_SetTextPosition(m_text, x, y);

    TTF_DrawRendererText(m_text, 0, 0);
}

void Font::setBold(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_BOLD) : (s & ~TTF_STYLE_BOLD));
}

void Font::setItalic(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_ITALIC) : (s & ~TTF_STYLE_ITALIC));
}

void Font::setUnderline(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_UNDERLINE) : (s & ~TTF_STYLE_UNDERLINE));
}

void Font::setStrikethrough(const bool on) const
{
    const unsigned int s = TTF_GetFontStyle(m_font);
    TTF_SetFontStyle(m_font, on ? (s | TTF_STYLE_STRIKETHROUGH) : (s & ~TTF_STYLE_STRIKETHROUGH));
}

void Font::setPtSize(int pt) const
{
    if (pt < 8)
        pt = 8;
    TTF_SetFontSize(m_font, static_cast<float>(pt));
}

namespace font
{
void _init()
{
    if (!TTF_Init())
        throw std::runtime_error("Failed to initialize SDL_ttf");

    _textEngine = TTF_CreateRendererTextEngine(renderer::_get());
    if (!_textEngine)
        throw std::runtime_error("Failed to create text engine: " + std::string(SDL_GetError()));
}

void _quit()
{
    if (_textEngine)
        TTF_DestroyRendererTextEngine(_textEngine);
    _textEngine = nullptr;

    TTF_Quit();
}

void _bind(const py::module_& module)
{
    py::classh<Font>(module, "Font", R"doc()doc")
        .def(py::init<const std::string&, int>(), R"doc()doc")
        .def(
            "render",
            [](const Font& self, const std::string& text, const py::object& posObj,
               const py::object& colorObj, const int wrapWidth) -> void
            {
                const auto pos = posObj.is_none() ? Vec2() : posObj.cast<Vec2>();
                const auto color =
                    colorObj.is_none() ? Color(255, 255, 255) : colorObj.cast<Color>();
                self.render(text, pos, color, wrapWidth);
            },
            py::arg("text"), py::arg("pos") = py::none(), py::arg("color") = py::none(),
            py::arg("wrap_width") = 0, R"doc()doc")
        .def("set_bold", &Font::setBold, py::arg("on"), R"doc()doc")
        .def("set_italic", &Font::setItalic, py::arg("on"), R"doc()doc")
        .def("set_underline", &Font::setUnderline, py::arg("on"), R"doc()doc")
        .def("set_strikethrough", &Font::setStrikethrough, py::arg("on"), R"doc()doc")
        .def("set_pt_size", &Font::setPtSize, py::arg("on"), R"doc()doc");
}
} // namespace font
} // namespace kn
