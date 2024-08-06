#pragma once

#include <SFML/Graphics.hpp>

#include "units.hpp"

#include <concepts>
#include <type_traits>

class Entity : public sf::Transformable, public sf::Drawable
{
  public:
	b2Body *body{};

	virtual void prePhysics()
	{
		if (body)
		{
			body->SetTransform(toBox2d(getPosition() / meterToPixels), getRotation().asRadians());
		}
	}

	virtual void postPhysics()
	{
		if (body)
		{
			setPosition(toSFML(body->GetPosition()) * meterToPixels);
			setRotation(sf::radians(body->GetAngle()));
		}
	}

	virtual void update(sf::Time deltaTime)
	{

	}

	void draw(sf::RenderTarget &target, sf::RenderStates states) const override
	{

	}

	virtual ~Entity() override = default;
};
template<typename T>
concept EntityDerivation = std::is_base_of_v<Entity, T>;