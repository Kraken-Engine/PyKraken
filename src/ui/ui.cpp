#include "kraken/ui/UI.hpp"

#include <stack>
#include <unordered_map>
#include <vector>

#include "kraken/core/Log.hpp"
#include "kraken/geometry/Collision.hpp"
#include "kraken/graphics/Camera.hpp"
#include "kraken/graphics/Draw.hpp"
#include "kraken/graphics/Renderer.hpp"
#include "kraken/graphics/Text.hpp"
#include "kraken/input/Input.hpp"
#include "kraken/input/Mouse.hpp"

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
static Camera* _prevCamera = nullptr;

static void _activateUICamera()
{
    _prevCamera = camera::_getActiveCamera();
    if (_prevCamera)
        _prevCamera->unset();
}

static void _deactivateUICamera()
{
    if (_prevCamera)
        _prevCamera->set();
    _prevCamera = nullptr;
}

static void _ensureActive();
static void _performLayout(Node* node);
static void _updateStateMap(Node* node);
static void _pushContainer(const Style& style, Direction dir);
static void _popContainer();
static size_t _generateId(const std::string& text);
static void _calculateSizes(Node* node);
static void _renderStyleBox(const Rect& bounds, const Style& style);
static void _begin(const Rect& rootBounds);
static void _end();

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

    virtual void measure() {}
    virtual void render() = 0;
};

class Box : public Node
{
  public:
    void render() override
    {
        _renderStyleBox(bounds, style);

        for (auto& child : children)
            child->render();
    }
};

class Label : public Node
{
  public:
    std::string text;

    Label(const std::string& t)
        : text(t)
    {
    }

    void measure() override
    {
        if (style.font)
        {
            if (!style.width || !style.height)
            {
                Text txt(*style.font, text);
                if (style.width)
                    txt.setWrapWidth(static_cast<int>(*style.width));
                Vec2 size = txt.getSize();
                if (!style.width)
                    bounds.w = size.x + style.padding * 2.0;
                if (!style.height)
                    bounds.h = size.y + style.padding * 2.0;
            }
        }
    }

    void render() override
    {
        _renderStyleBox(bounds, style);

        if (style.font)
        {
            Text txt(*style.font, text);
            txt.setWrapWidth(static_cast<int>(bounds.w));
            if (style.textColor)
                txt.setColor(*style.textColor);
            txt.draw(bounds.getCenter(), Anchor::CENTER);
        }
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

    void measure() override
    {
        if (style.font)
        {
            if (!style.width || !style.height)
            {
                Text txt(*style.font, text);
                if (style.width)
                    txt.setWrapWidth(static_cast<int>(*style.width));
                Vec2 size = txt.getSize();
                if (!style.width)
                    bounds.w = size.x + style.padding * 2.0;
                if (!style.height)
                    bounds.h = size.y + style.padding * 2.0;
            }
        }
    }

    void render() override
    {
        _renderStyleBox(bounds, style);

        if (_stateMap[id].isHovered)
        {
            static Color hoverColor{255, 255, 255, 40};
            draw::rect(bounds, hoverColor, 0, style.borderRadius);
        }

        if (style.font)
        {
            Text txt(*style.font, text);
            txt.setWrapWidth(static_cast<int>(bounds.w));
            if (style.textColor)
                txt.setColor(*style.textColor);

            txt.draw(bounds.getCenter(), Anchor::CENTER);
        }
    }
};

void _begin(const Rect& rootBounds)
{
    if (!_stack.empty())
        throw std::runtime_error("UI Error: A UI context is already active.");

    _activateUICamera();

    _root = std::make_unique<Box>();
    _root->id = _generateId("ROOT");
    _root->bounds = rootBounds;

    _root->style.width = rootBounds.w;
    _root->style.height = rootBounds.h;

    _stack.clear();
    _stack.push_back(_root.get());
    _containerIdCounter = 0;
}

void _end()
{
    _ensureActive();
    if (_stack.size() > 1)
    {
        _stack.clear();
        _deactivateUICamera();
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
    _deactivateUICamera();
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

    // 1. Generate ID
    const size_t id = _generateId("lbl_" + text);

    // 2. Create Node
    auto lbl = std::make_unique<Label>(text);
    lbl->id = id;
    lbl->style = style;
    _stack.back()->children.push_back(std::move(lbl));
}

void panel(const Style& style)
{
    _ensureActive();
    auto node = std::make_unique<Box>();
    node->id = ++_containerIdCounter;
    node->style = style;
    _stack.back()->children.push_back(std::move(node));
}

void Context::exit()
{
    if (m_active)
    {
        _popContainer();
        m_active = false;
    }
}

Context::~Context()
{
    exit();
}

RootContext::RootContext(const Rect& bounds, Direction dir, Align align, Align justify)
{
    _begin(bounds);
    if (_root)
    {
        _root->direction = dir;
        _root->align = align;
        _root->justify = justify;
    }
}

void RootContext::exit()
{
    if (m_active)
    {
        _end();
        m_active = false;
    }
}

RootContext::~RootContext()
{
    exit();
}

RootContext root(const Rect& bounds, Direction dir, Align align, Align justify)
{
    return RootContext(bounds, dir, align, justify);
}

Context row(
    const std::optional<Style>& style, double gap, double padding, Align align, Align justify
)
{
    Style s = style.value_or(Style{});
    if (gap != 0.0)
        s.gap = gap;
    if (padding != 0.0)
        s.padding = padding;

    _pushContainer(s, Direction::Horizontal);
    if (!_stack.empty())
    {
        auto* node = _stack.back();
        node->align = align;
        node->justify = justify;
    }
    return Context();
}

Context column(
    const std::optional<Style>& style, double gap, double padding, Align align, Align justify
)
{
    Style s = style.value_or(Style{});
    if (gap != 0.0)
        s.gap = gap;
    if (padding != 0.0)
        s.padding = padding;

    _pushContainer(s, Direction::Vertical);
    if (!_stack.empty())
    {
        auto* node = _stack.back();
        node->align = align;
        node->justify = justify;
    }
    return Context();
}

Context stack(const std::optional<Style>& style, double padding, Align align, Align justify)
{
    Style s = style.value_or(Style{});
    if (padding != 0.0)
        s.padding = padding;

    _pushContainer(s, Direction::Stack);
    if (!_stack.empty())
    {
        auto* node = _stack.back();
        node->align = align;
        node->justify = justify;
    }
    return Context();
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

void _renderStyleBox(const Rect& bounds, const Style& style)
{
    if (style.texture)
        renderer::draw9Slice(*style.texture, bounds, style.slice);
    else if (style.backgroundColor)
        draw::rect(bounds, *style.backgroundColor, 0, style.borderRadius);

    if (style.borderColor && style.borderWidth > 0)
    {
        draw::rect(
            bounds, *style.borderColor, style.borderWidth, style.texture ? 0.0 : style.borderRadius
        );
    }
}

void _calculateSizes(Node* node)
{
    if (!node)
        return;

    node->measure();

    double maxChildW = 0.0;
    double maxChildH = 0.0;
    double sumChildW = 0.0;
    double sumChildH = 0.0;
    size_t visibleChildren = node->children.size();

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

    const double gapSpace = visibleChildren == 0 ? 0.0 : node->style.gap * (visibleChildren - 1);
    const double padSpace = 2.0 * node->style.padding;

    const bool isHorizontal = (node->direction == Direction::Horizontal);
    const bool isStack = (node->direction == Direction::Stack);

    // Intrinsic Width
    if (node->style.width)
        node->bounds.w = *node->style.width;
    else if (!node->children.empty() || node->bounds.w == 0)
    {
        if (isStack)
            node->bounds.w = maxChildW + padSpace;
        else
            node->bounds.w = (isHorizontal ? sumChildW + gapSpace : maxChildW) + padSpace;
    }

    // Intrinsic Height
    if (node->style.height)
        node->bounds.h = *node->style.height;
    else if (!node->children.empty() || node->bounds.h == 0)
    {
        if (isStack)
            node->bounds.h = maxChildH + padSpace;
        else
            node->bounds.h = (isHorizontal ? maxChildH : sumChildH + gapSpace) + padSpace;
    }
}

void _performLayout(Node* node)
{
    if (!node || node->children.empty())
        return;

    const bool isHorizontal = (node->direction == Direction::Horizontal);
    const bool isStack = (node->direction == Direction::Stack);
    const double padding = node->style.padding;
    double gap = node->style.gap;

    const Rect contentArea =
        {node->bounds.x + padding, node->bounds.y + padding,
         std::max(0.0, node->bounds.w - 2.0 * padding),
         std::max(0.0, node->bounds.h - 2.0 * padding)};

    if (isStack)
    {
        for (auto& child : node->children)
        {
            const double margin = child->style.margin;

            child->bounds.x = contentArea.x + margin;
            child->bounds.y = contentArea.y + margin;

            if (node->justify == Align::Center)
                child->bounds.x += (contentArea.w - (child->bounds.w + margin * 2.0)) / 2.0;
            else if (node->justify == Align::End)
                child->bounds.x += contentArea.w - (child->bounds.w + margin * 2.0);
            else if (node->justify == Align::Stretch)
                child->bounds.w = std::max(0.0, contentArea.w - margin * 2.0);

            if (node->align == Align::Center)
                child->bounds.y += (contentArea.h - (child->bounds.h + margin * 2.0)) / 2.0;
            else if (node->align == Align::End)
                child->bounds.y += contentArea.h - (child->bounds.h + margin * 2.0);
            else if (node->align == Align::Stretch)
                child->bounds.h = std::max(0.0, contentArea.h - margin * 2.0);

            child->bounds.x += child->style.offset.x;
            child->bounds.y += child->style.offset.y;

            _performLayout(child.get());
        }
        return;
    }

    double totalMainLength = 0.0;
    size_t visibleChildren = node->children.size();
    for (const auto& child : node->children)
    {
        totalMainLength += (isHorizontal ? child->bounds.w : child->bounds.h) +
                           child->style.margin * 2.0;
    }
    if (visibleChildren > 0)
        totalMainLength += gap * (visibleChildren - 1);

    double currentPos = isHorizontal ? contentArea.x : contentArea.y;

    if (node->justify == Align::Center)
        currentPos += ((isHorizontal ? contentArea.w : contentArea.h) - totalMainLength) / 2.0;
    else if (node->justify == Align::End)
        currentPos += (isHorizontal ? contentArea.w : contentArea.h) - totalMainLength;
    else if (node->justify == Align::Stretch && visibleChildren > 0)
    {
        // Calculate stretch gap to divide extra space completely evenly
        // Not perfectly implemented for flex-grow on items themslves, but rather spaces between
        // them.
        double extraSpace =
            std::max(0.0, (isHorizontal ? contentArea.w : contentArea.h) - totalMainLength);
        if (visibleChildren > 1)
            gap = gap + (extraSpace / (visibleChildren - 1));
    }

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

        child->bounds.x += child->style.offset.x;
        child->bounds.y += child->style.offset.y;

        _performLayout(child.get());
    }
}

}  // namespace kn::ui
