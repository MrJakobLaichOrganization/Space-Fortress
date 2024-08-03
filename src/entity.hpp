#pragma once

#include <SFML/Graphics.hpp>

class Entity : public sf::Transformable, public sf::Drawable
{
  public:

	virtual void Update(sf::Time deltaTime)
	{

	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{

	}

	virtual ~Entity() = default;


};