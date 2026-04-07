#include "ui/UI.hpp"

#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>

#include <algorithm>
#include <stack>
#include <vector>

#include "Input.hpp"
#include "Log.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"

namespace kn::ui
{
static std::vector<Node*> _stack;
static std::unique_ptr<Node> _root;

static void _ensureActive()
{
    if (_stack.empty())
        throw std::runtime_error("UI Error: No active UI context.");
}

static void _performLayout(Node* node);
static void _pushContainer(const Style& style, Direction dir);
static void _popContainer();

void Box::render()
{
    if (style.texture)
        renderer::draw9Slice(*style.texture, bounds, style.slice);
    // TODO: Draw background color if texture is null

    for (auto& child : children)
        child->render();
}

bool Box::handleInput(const Vec2& mousePos, bool clicked)
{
    for (auto it = children.rbegin(); it != children.rend(); ++it)
    {
        if ((*it)->handleInput(mousePos, clicked))
            return true;
    }
    return false;
}

void begin(const Rect& rootBounds)
{
    if (!_stack.empty())
        throw std::runtime_error("UI Error: A UI context is already active.");

    _root = std::make_unique<Box>();
    _root->bounds = rootBounds;
    _stack.clear();
    _stack.push_back(_root.get());
}

void end()
{
    _ensureActive();
    if (_stack.size() > 1)
        throw std::runtime_error(
            "UI Error: Imbalanced UI stack. Some containers (rows/columns) were not closed."
        );

    if (_root)
    {
        _performLayout(_root.get());
        _root->handleInput(mouse::getPos(), mouse::isJustPressed(MouseButton::Left));
        _root->render();
    }

    _stack.clear();
}

bool button(const std::string& text, const Style& style)
{
    _ensureActive();
    // Simplified: For now use Box as button base
    // This will need a dedicated Button node for state management
    return false;
}

void label(const std::string& text, const Style& style)
{
    _ensureActive();
}

void image(const Texture& tex, const Rect& slice, const Style& style)
{
    _ensureActive();
}

void _pushContainer(const Style& style, Direction dir)
{
    _ensureActive();
    auto box = std::make_unique<Box>();
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

    double totalFixedSize = 0;
    for (auto& child : node->children)
    {
        double childW = child->style.width.value_or(child->bounds.w);
        double childH = child->style.height.value_or(child->bounds.h);

        child->bounds.w = childW;
        child->bounds.h = childH;

        if (isHorizontal)
            totalFixedSize += childW;
        else
            totalFixedSize += childH;
    }

    if (!node->children.empty())
        totalFixedSize += gap * (node->children.size() - 1);

    double currentPos = isHorizontal ? contentArea.x : contentArea.y;

    for (auto& child : node->children)
    {
        if (isHorizontal)
        {
            child->bounds.x = currentPos;
            if (node->align == Align::Center)
                child->bounds.y = contentArea.y + (contentArea.h - child->bounds.h) / 2.0;
            else if (node->align == Align::End)
                child->bounds.y = contentArea.getBottom() - child->bounds.h;
            else if (node->align == Align::Stretch)
            {
                child->bounds.y = contentArea.y;
                child->bounds.h = contentArea.h;
            }
            else
                child->bounds.y = contentArea.y;

            currentPos += child->bounds.w + gap;
        }
        else
        {
            child->bounds.y = currentPos;
            if (node->align == Align::Center)
                child->bounds.x = contentArea.x + (contentArea.w - child->bounds.w) / 2.0;
            else if (node->align == Align::End)
                child->bounds.x = contentArea.getRight() - child->bounds.w;
            else if (node->align == Align::Stretch)
            {
                child->bounds.x = contentArea.x;
                child->bounds.w = contentArea.w;
            }
            else
                child->bounds.x = contentArea.x;

            currentPos += child->bounds.h + gap;
        }

        _performLayout(child.get());
    }
}

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
                std::optional<Color>, const Texture*, Rect, double, double, double,
                std::optional<double>, std::optional<double>>(),
            "background_color"_a = nb::none(), "texture"_a = nb::none(), "slice"_a = Rect{},
            "padding"_a = 0.0, "gap"_a = 0.0, "margin"_a = 0.0, "width"_a = nb::none(),
            "height"_a = nb::none()
        )
        .def_rw("background_color", &Style::backgroundColor)
        .def_rw("texture", &Style::texture)
        .def_rw("slice", &Style::slice)
        .def_rw("padding", &Style::padding)
        .def_rw("gap", &Style::gap)
        .def_rw("margin", &Style::margin)
        .def_rw("width", &Style::width)
        .def_rw("height", &Style::height);

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
        void __exit__(nb::handle, nb::handle, nb::handle)
        {
            end();
        }
    };

    nb::class_<ContextProxy>(subUI, "_ContextProxy")
        .def("__enter__", &ContextProxy::__enter__)
        .def("__exit__", &ContextProxy::__exit__);

    nb::class_<RootProxy, ContextProxy>(subUI, "_RootProxy")
        .def("__enter__", &RootProxy::__enter__)
        .def("__exit__", &RootProxy::__exit__);

    subUI.def(
        "root",
        [](const Rect& bounds, Direction dir, Align align)
        {
            auto p = RootProxy(bounds);
            _root->direction = dir;
            _root->align = align;
            return p;
        },
        "bounds"_a, "direction"_a = Direction::Vertical, "align"_a = Align::Start
    );

    subUI.def(
        "row",
        [](double gap, double padding, Align align)
        {
            Style s;
            s.gap = gap;
            s.padding = padding;
            _pushContainer(s, Direction::Horizontal);
            _stack.back()->align = align;
            return ContextProxy();
        },
        "gap"_a = 0.0, "padding"_a = 0.0, "align"_a = Align::Start
    );

    subUI.def(
        "column",
        [](double gap, double padding, Align align)
        {
            Style s;
            s.gap = gap;
            s.padding = padding;
            _pushContainer(s, Direction::Vertical);
            _stack.back()->align = align;
            return ContextProxy();
        },
        "gap"_a = 0.0, "padding"_a = 0.0, "align"_a = Align::Start
    );

    subUI.def("button", &button, "text"_a, "style"_a = Style{});
    subUI.def("label", &label, "text"_a, "style"_a = Style{});
    subUI.def("image", &image, "texture"_a, "slice"_a = Rect{}, "style"_a = Style{});
}
}  // namespace kn::ui
