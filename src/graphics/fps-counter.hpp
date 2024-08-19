#pragma once

#include "SFML/Graphics.hpp"
#include "resources.hpp"

#include <cmath>

class FpsCountrer : public sf::Drawable
{
    sf::Text m_text{Resources::get().imbPlexMono};

public:
    void update(sf::Time deltaTime)
    {
        m_text.setString({std::to_string(std::round(1.f / deltaTime.asSeconds()))});
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.setView({sf::Vector2f{target.getSize() / 2u}, sf::Vector2f{target.getSize()}});
        target.draw(m_text, states);
    }
};