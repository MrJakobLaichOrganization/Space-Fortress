#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/System/Vector2.hpp>

#include <functional>
#include <inputmanager.hpp>
#include <memory>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

#include <cstdint>

class GuiElement : public sf::Drawable
{
public:
    enum Features : std::uint8_t
    {
        FEAT_CLICKABLE = 1 << 0,
        FEAT_CLICKPASS = 1 << 1
    };

protected:
    GuiElement* m_parent;
    sf::Vector2f m_position;
    sf::Vector2f m_dims;
    std::vector<std::unique_ptr<GuiElement>> m_children;
    Features m_features;
    std::int32_t m_zPos; // The bigger zpos the topmost it is

    GuiElement(sf::Vector2f position, sf::Vector2f dims, Features feats, GuiElement* parent = nullptr) :
        m_position{position},
        m_dims{dims},
        m_parent{parent},
        m_features{feats},
        m_zPos(parent ? parent->getZpos() + 1 : 0)
    {
    }
    GuiElement(sf::Vector2f position, sf::Vector2f dims, GuiElement* parent = nullptr) :
        GuiElement(position, dims, static_cast<Features>(0), parent)
    {
    }

    // Normalized center [0-1]
    [[nodiscard]] sf::Vector2f calculateCenterNormalized() const
    {
        return sf::Vector2f{m_position.x + m_dims.x / 2.f, m_position.y + m_dims.y / 2.f};
    }

    // Center in pixels
    [[nodiscard]] sf::Vector2f calculateCenter() const
    {
        auto position = pixelPos();
        auto size = pixelSize();
        return sf::Vector2f{position.x + size.x / 2.f, position.y + size.y / 2.f};
    }

    [[nodiscard]] virtual sf::Vector2u getScreenDims() const
    {
        return m_parent->getScreenDims();
    }

public:
    virtual ~GuiElement() = default;

    template <typename T, typename... Args>
    T& addChild(Args&&... args)
    {
        m_children.push_back(std::make_unique<T>(std::forward<Args>(args)..., this));
        onChildAdded(m_children.back().get());
        return static_cast<T&>(*m_children.back());
    }

    virtual void update(InputManager& manager)
    {
        for (auto& child : m_children)
        {
            child->update(manager);
        }
    }
    virtual void onResize(sf::Vector2u newDims)
    {
        for (auto& child : m_children)
        {
            child->onResize(newDims);
        }
    }
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for (auto& child : m_children)
        {
            child->draw(target, states);
        }
    }
    virtual void onClick(InputManager& manager)
    {
    }
    virtual void onChildAdded(GuiElement* el)
    {
        if (m_parent)
        {
            m_parent->onChildAdded(el);
        }
    }

    [[nodiscard]] bool hasFeature(Features feat) const
    {
        return (m_features & feat) != 0;
    }
    void setFeature(Features feat, bool on = true)
    {
        if (on)
        {
            m_features = static_cast<Features>(std::to_underlying(m_features) | std::to_underlying(feat));
        }
        else
        {
            m_features = static_cast<Features>(std::to_underlying(m_features) ^ ~std::to_underlying(feat));
        }
    }

    // Effective pos of element in pixels
    [[nodiscard]] virtual sf::Vector2f pixelPos() const
    {
        auto parentPos = m_parent->pixelPos();
        auto parentDims = m_parent->pixelSize();
        return {parentPos.x + parentDims.x * m_position.x, parentPos.y + parentDims.y * m_position.y};
    }
    // Effective size of element in pixels
    [[nodiscard]] virtual sf::Vector2f pixelSize() const
    {
        auto parentSz = m_parent->pixelSize();
        return {parentSz.x * m_dims.x, parentSz.y * m_dims.y};
    }
    [[nodiscard]] std::int32_t getZpos() const
    {
        return m_zPos;
    }

    struct ZposSorter
    {
        constexpr bool operator()(const GuiElement* first, const GuiElement* second) const
        {
            return first->getZpos() > second->getZpos();
        }
    };
};

// Empty container
class MainContainer : public GuiElement
{
private:
    std::set<GuiElement*, GuiElement::ZposSorter> m_allElements;

public:
    MainContainer(sf::Vector2u screenDims) : m_screenDims{screenDims}, GuiElement({}, {1.f, 1.f})
    {
    }

    [[nodiscard]] sf::Vector2f pixelPos() const override
    {
        return sf::Vector2f{m_screenDims.x * m_position.x, m_screenDims.y * m_position.y};
    }
    [[nodiscard]] sf::Vector2f pixelSize() const override
    {
        return sf::Vector2f{m_screenDims.x * m_dims.x, m_screenDims.y * m_dims.y};
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        GuiElement::draw(target, states);
    }
    void update(InputManager& manager) override
    {
        GuiElement::update(manager);
    }
    void onResize(sf::Vector2u newDims) override
    {
        m_screenDims = newDims;

        GuiElement::onResize(newDims);
    }
    void onClick(InputManager& manager) override
    {
        for (auto& element : m_allElements)
        {
            if (!element->hasFeature(FEAT_CLICKABLE))
                continue;
            if(manager.screenMousePos.x < element->pixelPos().x || manager.screenMousePos.y > element->pixelPos().x + element->pixelSize().x)
            {
                continue;
            }
            if(manager.screenMousePos.y < element->pixelPos().y || manager.screenMousePos.y > element->pixelPos().y + element->pixelSize().y)
            {
                continue;
            }

            element->onClick(manager);
            if (!element->hasFeature(FEAT_CLICKPASS))
            {
                break;
            }
        }
    }
    void onChildAdded(GuiElement* child) override
    {
        GuiElement::onChildAdded(child);
        m_allElements.insert(child);
    }

protected:
    sf::Vector2u m_screenDims;
    [[nodiscard]] sf::Vector2u getScreenDims() const override
    {
        return m_screenDims;
    }
};

class Button : public GuiElement
{
public:
    Button(sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent = nullptr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(InputManager& input) override;
    void onResize(sf::Vector2u newDims) override;
    void onClick(InputManager& manager) override;

    void setHover(std::function<void()> callback)
    {
        m_onHover = callback;
    }
    void setClick(std::function<void()> callback)
    {
        m_onClick = callback;
    }

    void setPos(sf::Vector2f position);

private:
    std::function<void()> m_onHover;
    std::function<void()> m_onClick;

    const sf::Texture m_bgTx{sf::Texture::loadFromFile(ASSETS_DIR "/gui/button_bg.png").value()};
    sf::Sprite m_background{m_bgTx};

    bool m_lastPressed{};
    bool m_hovering{};
};

class Image : public GuiElement
{
public:
    Image(std::string_view imgPath, sf::Vector2f position, sf::Vector2f dims, GuiElement* parent = nullptr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onResize(sf::Vector2u newDims) override;

    void setPos(sf::Vector2f newPos);
    void setSize(sf::Vector2f newSize);
    void setImage(std::string_view imgPath);

private:
    void updateImageDimensions();

    sf::Texture m_tx;
    sf::Sprite m_sprite;
};

enum class TextCentering : std::uint8_t
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    CENTER
};
class Text : public GuiElement
{
public:
    Text(std::string_view text,
         const sf::Font& font,
         sf::Vector2f pos,
         sf::Vector2f dims,
         TextCentering horizontalCentering,
         TextCentering verticalCentering,
         GuiElement* parent = nullptr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onResize(sf::Vector2u newDims) override;

    [[nodiscard]] const std::string& getText() const
    {
        return m_text;
    }
    void setText(std::string_view text);

private:
    static constexpr std::uint32_t yPadding = 5;

    void updateDimensions();

    TextCentering m_horizontalCentering;
    TextCentering m_verticalCentering;
    std::string m_text;
    const sf::Font& m_font;
    std::vector<sf::Text> m_lines;
};