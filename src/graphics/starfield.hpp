#pragma once

#include <SFML/graphics.hpp>

class Starfield : public sf::Drawable
{
    mutable sf::Shader m_starFieldShader = sf::Shader::loadFromFile(ASSETS_DIR "/shaders/starfield.frag", sf::Shader::Type::Fragment)
                                       .value();
    float m_starfieldTime{};

public:
    void update(sf::Time deltaTime, sf::Vector2f viewCenter, float viewZoom)
    {
        m_starfieldTime += deltaTime.asSeconds();

        m_starFieldShader.setUniform("iZoom", 0.005f / viewZoom);
        m_starFieldShader.setUniform("iOffset", viewCenter / 100000.f);
        m_starFieldShader.setUniform("iTime", m_starfieldTime);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        const sf::Vector2f targetSize{target.getSize()};
        target.setView(sf::View{targetSize / 2.f, targetSize});

        m_starFieldShader.setUniform("iResolution", targetSize);

        sf::RectangleShape startfieldShape{targetSize};
        startfieldShape.setTextureRect({{0, 0}, {1, 1}});

        states.shader = &m_starFieldShader;

        target.draw(startfieldShape, states);
    }
};