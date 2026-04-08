#include "ui/UI.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>

#include <stack>
#include <unordered_map>
#include <vector>

#include "Collision.hpp"
#include "Draw.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Math.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "Text.hpp"

namespace kn::ui
{
class Node;

struct ElementState
{
    Rect lastBounds;
    bool isHovered = false;
    bool isClicked = false;
};

static std::unordered_map<size_t, ElementState> _stateMap;
static size_t _containerIdCounter = 0;

static std::vector<Node*> _stack;
static std::unique_ptr<Node> _root;

static void _ensureActive();
static void _performLayout(Node* node);
static void _updateStateMap(Node* node);
static void _pushContainer(const Style& style, Direction dir);
static void _popContainer();
static size_t _generateId(const std::string& text);
static void _calculateSizes(Node* node);

class Node
{
  public:
    Node() = default;
    virtual ~Node() = default;

    size_t id = 0;
    Style style;
    Rect bounds;
    std::vector<std::unique_ptr<Node>> children;

    // Layout properties
    Direction direction = Direction::Vertical;
    Align align = Align::Start;
    Align justify = Align::Start;

    virtual void render() = 0;
};

class Box : public Node
{
  public:
    void render() override
    {
        if (style.texture)
            renderer::draw9Slice(*style.texture, bounds, style.slice);
        else if (style.backgroundColor)
            draw::rect(bounds, *style.backgroundColor);

        for (auto& child : children)
            child->render();
    }
};

class Image : public Node
{
  public:
    Image(const Texture* tex, const Rect& slice)
    {
        style.texture = tex;
        style.slice = slice;
    }

    void render() override
    {
        if (style.texture)
            renderer::draw9Slice(*style.texture, bounds, style.slice);
        else if (style.backgroundColor)
            draw::rect(bounds, *style.backgroundColor);
    }
};

class Button : public Node
{
  public:
    std::string text;

    Button(const std::string& t)
        : text(t)
    {
    }

    void render() override
    {
        // 1. Draw Background
        if (style.texture)
            renderer::draw9Slice(*style.texture, bounds, style.slice);
        else if (style.backgroundColor)
            draw::rect(bounds, *style.backgroundColor, 0, style.borderRadius);

        if (style.borderColor && style.borderWidth > 0)
        {
            draw::rect(
                bounds, *style.borderColor, style.borderWidth,
                style.texture ? 0.0 : style.borderRadius
            );
        }

        // 2. Draw Hover Effect
        if (_stateMap[id].isHovered)
        {
            static Color hoverColor{255, 255, 255, 40};
            draw::rect(bounds, hoverColor, 0, style.texture ? 0.0 : style.borderRadius);
        }

        // 3. Draw Text
        if (style.font)
        {
            Text txt(*style.font, text);
            txt.setWrapWidth(static_cast<int>(bounds.w));
            txt.draw(bounds.getCenter(), Anchor::CENTER);
        }
    }
};

void begin(const Rect& rootBounds)
{
    if (!_stack.empty())
        throw std::runtime_error("UI Error: A UI context is already active.");

    _root = std::make_unique<Box>();
    _root->id = _generateId("ROOT");
    _root->bounds = rootBounds;

    _root->style.width = rootBounds.w;
    _root->style.height = rootBounds.h;

    _stack.clear();
    _stack.push_back(_root.get());
    _containerIdCounter = 0;
}

void end()
{
    _ensureActive();
    if (_stack.size() > 1)
    {
        _stack.clear();
        throw std::runtime_error(
            "UI Error: Imbalanced UI stack. Some containers (rows/columns) were not closed."
        );
    }

    if (_root)
    {
        _calculateSizes(_root.get());
        _performLayout(_root.get());
        _updateStateMap(_root.get());
        _root->render();
    }

    _stack.clear();
}

bool button(const std::string& text, const Style& style)
{
    _ensureActive();

    // 1. Generate ID
    const size_t id = _generateId("btn_" + text);

    // 2. Create Node
    auto btn = std::make_unique<Button>(text);
    btn->id = id;
    btn->style = style;
    _stack.back()->children.push_back(std::move(btn));

    // 3. Evaluate Input using LAST frame's bounds
    bool clicked = false;
    if (_stateMap.find(id) != _stateMap.end())
    {
        const Rect lastBounds = _stateMap[id].lastBounds;
        const Vec2 mousePos = mouse::getPos();

        if (collision::overlap(lastBounds, mousePos))
        {
            _stateMap[id].isHovered = true;
            if (mouse::isJustPressed(MouseButton::Left))
            {
                clicked = true;
                _stateMap[id].isClicked = true;
            }
        }
        else
        {
            _stateMap[id].isHovered = false;
            _stateMap[id].isClicked = false;
        }
    }

    return clicked;
}

void label(const std::string& text, const Style& style)
{
    _ensureActive();
    // Similar to button, but without the input checking logic
    // Create a LabelNode, push to children, etc.
}

void image(const Texture* tex, const Rect& slice, const Style& style)
{
    _ensureActive();

    auto img = std::make_unique<Image>(tex, slice);
    img->id = ++_containerIdCounter;  // Throwaway ID
    img->style = style;
    _stack.back()->children.push_back(std::move(img));
}

void _ensureActive()
{
    if (_stack.empty())
        throw std::runtime_error("UI Error: No active UI context.");
}

void _pushContainer(const Style& style, Direction dir)
{
    _ensureActive();
    auto box = std::make_unique<Box>();
    box->id = ++_containerIdCounter;  // Generic ID
    box->style = style;
    box->direction = dir;

    Node* parent = _stack.back();
    Node* ptr = box.get();
    parent->children.push_back(std::move(box));
    _stack.push_back(ptr);
}

void _popContainer()
{
    _ensureActive();
    if (_stack.size() > 1)
        _stack.pop_back();
    else
        throw std::runtime_error("UI Error: Attempted to pop the root container.");
}

void _updateStateMap(Node* node)
{
    if (!node)
        return;

    // Save the newly calculated bounds into the map
    if (node->id != 0)
        _stateMap[node->id].lastBounds = node->bounds;

    for (auto& child : node->children)
        _updateStateMap(child.get());
}

size_t _generateId(const std::string& text)
{
    size_t hash = std::hash<std::string>{}(text);

    // Mix in the parent ID to allow buttons with the same name in different menus
    if (!_stack.empty() && _stack.back() != nullptr)
    {
        size_t parentId = _stack.back()->id;
        size_t siblingIndex = _stack.back()->children.size();

        hash ^= parentId + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= siblingIndex + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }

    return hash;
}

void _calculateSizes(Node* node)
{
    if (!node)
        return;

    double maxChildW = 0.0;
    double maxChildH = 0.0;
    double sumChildW = 0.0;
    double sumChildH = 0.0;

    for (auto& child : node->children)
    {
        _calculateSizes(child.get());

        const double childW = child->bounds.w + child->style.margin * 2.0;
        const double childH = child->bounds.h + child->style.margin * 2.0;

        maxChildW = std::max(maxChildW, childW);
        maxChildH = std::max(maxChildH, childH);
        sumChildW += childW;
        sumChildH += childH;
    }

    const double gapSpace = node->children.empty() ? 0.0
                                                   : node->style.gap * (node->children.size() - 1);
    const double padSpace = 2.0 * node->style.padding;

    const bool isHorizontal = (node->direction == Direction::Horizontal);

    // Intrinsic Width
    if (node->style.width)
        node->bounds.w = *node->style.width;
    else
        node->bounds.w = (isHorizontal ? sumChildW + gapSpace : maxChildW) + padSpace;

    // Intrinsic Height
    if (node->style.height)
        node->bounds.h = *node->style.height;
    else
        node->bounds.h = (isHorizontal ? maxChildH : sumChildH + gapSpace) + padSpace;
}

void _performLayout(Node* node)
{
    if (!node || node->children.empty())
        return;

    const bool isHorizontal = (node->direction == Direction::Horizontal);
    const double padding = node->style.padding;
    const double gap = node->style.gap;

    const Rect contentArea =
        {node->bounds.x + padding, node->bounds.y + padding,
         std::max(0.0, node->bounds.w - 2.0 * padding),
         std::max(0.0, node->bounds.h - 2.0 * padding)};

    double totalMainLength = 0.0;
    for (const auto& child : node->children)
        totalMainLength += (isHorizontal ? child->bounds.w : child->bounds.h) +
                           child->style.margin * 2.0;
    if (!node->children.empty())
        totalMainLength += gap * (node->children.size() - 1);

    double currentPos = isHorizontal ? contentArea.x : contentArea.y;

    if (node->justify == Align::Center)
        currentPos += ((isHorizontal ? contentArea.w : contentArea.h) - totalMainLength) / 2.0;
    else if (node->justify == Align::End)
        currentPos += (isHorizontal ? contentArea.w : contentArea.h) - totalMainLength;

    for (auto& child : node->children)
    {
        const double margin = child->style.margin;
        if (isHorizontal)
        {
            child->bounds.x = currentPos + margin;
            if (node->align == Align::Center)
                child->bounds.y = contentArea.y + margin +
                                  (contentArea.h - (child->bounds.h + margin * 2.0)) / 2.0;
            else if (node->align == Align::End)
                child->bounds.y = contentArea.getBottom() - child->bounds.h - margin;
            else if (node->align == Align::Stretch)
            {
                child->bounds.y = contentArea.y + margin;
                child->bounds.h = std::max(0.0, contentArea.h - margin * 2.0);
            }
            else
                child->bounds.y = contentArea.y + margin;

            currentPos += child->bounds.w + margin * 2.0 + gap;
        }
        else
        {
            child->bounds.y = currentPos + margin;
            if (node->align == Align::Center)
                child->bounds.x = contentArea.x + margin +
                                  (contentArea.w - (child->bounds.w + margin * 2.0)) / 2.0;
            else if (node->align == Align::End)
                child->bounds.x = contentArea.getRight() - child->bounds.w - margin;
            else if (node->align == Align::Stretch)
            {
                child->bounds.x = contentArea.x + margin;
                child->bounds.w = std::max(0.0, contentArea.w - margin * 2.0);
            }
            else
                child->bounds.x = contentArea.x + margin;

            currentPos += child->bounds.h + margin * 2.0 + gap;
        }

        _performLayout(child.get());
    }
}

struct ContextProxy
{
    ContextProxy() = default;
    void __enter__() {}
    void __exit__(nb::handle, nb::handle, nb::handle)
    {
        _popContainer();
    }
};

struct RootProxy : public ContextProxy
{
    RootProxy(const Rect& bounds)
    {
        begin(bounds);
    }
    void __exit__(nb::handle type, nb::handle value, nb::handle traceback)
    {
        end();
    }
};

void _bind(nb::module_& m)
{
    using namespace nb::literals;

    auto subUI = m.def_submodule("ui", "A declarative UI and layout submodule.");

    nb::enum_<Direction>(subUI, "Direction")
        .value("HORIZONTAL", Direction::Horizontal)
        .value("VERTICAL", Direction::Vertical);

    nb::enum_<Align>(subUI, "Align")
        .value("START", Align::Start)
        .value("CENTER", Align::Center)
        .value("END", Align::End)
        .value("STRETCH", Align::Stretch);

    nb::class_<Style>(subUI, "Style")
        .def(
            nb::init<
                std::optional<Color>, const Texture*, Rect, const Font*, std::optional<Color>,
                double, double, double, int, double, std::optional<Color>, std::optional<double>,
                std::optional<double>>(),
            "background_color"_a = nb::none(), "texture"_a = nb::none(), "slice"_a = Rect{},
            "font"_a = nb::none(), "text_color"_a = nb::none(), "padding"_a = 0.0, "margin"_a = 0.0,
            "gap"_a = 0.0, "border_width"_a = 0, "border_radius"_a = 0.0,
            "border_color"_a = nb::none(), "width"_a = nb::none(), "height"_a = nb::none()
        )
        .def_rw("background_color", &Style::backgroundColor)
        .def_rw("texture", &Style::texture)
        .def_rw("slice", &Style::slice)
        .def_rw("font", &Style::font)
        .def_rw("text_color", &Style::textColor)
        .def_rw("padding", &Style::padding)
        .def_rw("margin", &Style::margin)
        .def_rw("gap", &Style::gap)
        .def_rw("border_width", &Style::borderWidth)
        .def_rw("border_radius", &Style::borderRadius)
        .def_rw("border_color", &Style::borderColor)
        .def_rw("width", &Style::width)
        .def_rw("height", &Style::height);

    nb::class_<ContextProxy>(subUI, "_ContextProxy")
        .def("__enter__", &ContextProxy::__enter__)
        .def(
            "__exit__", &ContextProxy::__exit__, nb::arg("type").none(), nb::arg("value").none(),
            nb::arg("traceback").none()
        );

    nb::class_<RootProxy, ContextProxy>(subUI, "_RootProxy")
        .def("__enter__", &RootProxy::__enter__)
        .def(
            "__exit__", &RootProxy::__exit__, nb::arg("type").none(), nb::arg("value").none(),
            nb::arg("traceback").none()
        );

    subUI.def(
        "root",
        [](const Rect& bounds, const Direction dir, const Align align, const Align justify)
        {
            auto p = RootProxy(bounds);
            _root->direction = dir;
            _root->align = align;
            _root->justify = justify;
            return p;
        },
        "bounds"_a, "direction"_a = Direction::Vertical, "align"_a = Align::Start,
        "justify"_a = Align::Start
    );

    subUI.def(
        "row",
        [](const double gap, const double padding, const Align align, const Align justify)
        {
            Style s;
            s.gap = gap;
            s.padding = padding;
            _pushContainer(s, Direction::Horizontal);
            _stack.back()->align = align;
            _stack.back()->justify = justify;
            return ContextProxy();
        },
        "gap"_a = 0.0, "padding"_a = 0.0, "align"_a = Align::Start, "justify"_a = Align::Start
    );

    subUI.def(
        "column",
        [](const double gap, const double padding, const Align align, const Align justify)
        {
            Style s;
            s.gap = gap;
            s.padding = padding;
            _pushContainer(s, Direction::Vertical);
            _stack.back()->align = align;
            _stack.back()->justify = justify;
            return ContextProxy();
        },
        "gap"_a = 0.0, "padding"_a = 0.0, "align"_a = Align::Start, "justify"_a = Align::Start
    );

    subUI.def("button", &button, "text"_a, "style"_a = Style{});
    subUI.def("label", &label, "text"_a, "style"_a = Style{});
    subUI.def("image", &image, "texture"_a = nb::none(), "slice"_a = Rect{}, "style"_a = Style{});
}
}  // namespace kn::ui
