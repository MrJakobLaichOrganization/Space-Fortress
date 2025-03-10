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
#include <string_view>
#include <vector>

#include <cstdint>

class GuiElement : public sf::Drawable
{
protected:
    GuiElement* m_parent;
    sf::Vector2f m_position;
    sf::Vector2f m_dims;
    std::vector<std::unique_ptr<GuiElement>> children;

    GuiElement(sf::Vector2f position, sf::Vector2f dims, GuiElement* parent = nullptr) :
        m_position{position},
        m_dims{dims},
        m_parent{parent}
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

    void drawImpl(sf::RenderWindow& wind)
    {
        wind.draw(*this);

        for (auto& child : children)
        {
            child->drawImpl(wind);
        }
    }
    void updateImpl(InputManager& inp)
    {
        update(inp);
        for (auto& child : children)
        {
            child->updateImpl(inp);
        }
    }
    void onResizeImpl(sf::Vector2u newDims)
    {
        onResize(newDims);
        for (auto& child : children)
        {
            child->onResizeImpl(newDims);
        }
    }

    template <typename T, typename... Args>
    T& addChild(Args&&... args)
    {
        children.push_back(std::make_unique<T>(std::forward<Args>(args)..., this));
        return static_cast<T&>(*children.back());
    }

    virtual void update(InputManager& input)
    {
    }
    // On screen resize (do not call this manually)
    virtual void onResize(sf::Vector2u newDims)
    {
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
};

// Empty container
class MainContainer : public GuiElement
{
public:
    MainContainer(sf::Vector2u screenDims) : m_screenDims{screenDims}, GuiElement({}, {1.f, 1.f})
    {
    }

    void onResize(sf::Vector2u newSize) override
    {
        m_screenDims = newSize;
    }

    [[nodiscard]] sf::Vector2f pixelPos() const override
    {
        return sf::Vector2f{m_screenDims.x * m_position.x, m_screenDims.y * m_position.y};
    }
    [[nodiscard]] sf::Vector2f pixelSize() const override
    {
        return sf::Vector2f{m_screenDims.x * m_dims.x, m_screenDims.y * m_dims.y};
    }

    void draw(sf::RenderTarget& _target, sf::RenderStates _states) const override
    {
    }

protected:
    sf::Vector2u m_screenDims;
    [[nodiscard]] sf::Vector2u getScreenDims() const override
    {
        return m_screenDims;
    }
};

class TextButton : public GuiElement
{
public:
    TextButton(std::string_view text, const sf::Font& font, sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent = nullptr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(InputManager& input) override;
    void onResize(sf::Vector2u newDims) override;

    void setHover(std::function<void()> callback)
    {
        m_onHover = callback;
    }
    void setClick(std::function<void()> callback)
    {
        m_onClick = callback;
    }

    void setPos(sf::Vector2f position);
    void setText(std::string_view text);

private:
    void updateTextDimensions();

    std::function<void()> m_onHover;
    std::function<void()> m_onClick;

    const sf::Texture m_bgTx{sf::Texture::loadFromFile(ASSETS_DIR "/Gui/button_bg.png").value()};
    sf::Sprite m_background{m_bgTx};

    sf::Text m_text;
    bool m_lastPressed{};
    bool m_hovering{};
};
class ImageButton : public GuiElement
{
public:
    ImageButton(std::string_view imgDir, sf::Vector2f loc, sf::Vector2f dim, GuiElement* parent = nullptr);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void update(InputManager& input) override;
    void onResize(sf::Vector2u newDims) override;

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
    void updateImageDimensions();

    std::function<void()> m_onHover;
    std::function<void()> m_onClick;

    const sf::Texture m_bgTx{sf::Texture::loadFromFile(ASSETS_DIR "/Gui/button_bg.png").value()};
    sf::Sprite m_background{m_bgTx};

    sf::Texture m_spriteTx;
    sf::Sprite m_img;
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

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
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