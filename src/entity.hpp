#pragma once

#include <SFML/Graphics.hpp>

#include "units.hpp"

#include <concepts>
#include <type_traits>

class Entity : public sf::Transformable, public sf::Drawable
{
  public:
	b2Body *body{};

	virtual void PrePhysics()
	{
		if (body)
		{
			body->SetTransform(toBox2d(getPosition() / MeterToPixels), getRotation().asRadians());
		}
	}

	virtual void PostPhysics()
	{
		if (body)
		{
			setPosition(toSFML(body->GetPosition()) * MeterToPixels);
			setRotation(sf::radians(body->GetAngle()));
		}
	}

	virtual void Update(sf::Time deltaTime)
	{

	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{

	}

	virtual ~Entity() = default;
};
template<typename T>
concept EntityDerivation = std::is_base_of_v<Entity, T>;