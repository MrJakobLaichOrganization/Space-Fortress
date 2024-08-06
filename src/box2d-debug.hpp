#pragma once
 
#include <box2d/box2d.h>

#include <SFML/Graphics.hpp>

#include "units.hpp"

class PhysicsDebugDraw : public b2Draw
{
  public:
	PhysicsDebugDraw(sf::RenderTarget* target) : target{ target }
	{

	}

	sf::RenderTarget* target;

	constexpr static float outlineWidth = 3.f;

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::ConvexShape shape(vertexCount);
		for (int32 x = 0; x < vertexCount; x++)
		{
			shape.setPoint(x, {vertices[x].x * meterToPixels, vertices[x].y * meterToPixels});
		}

		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		shape.setOutlineThickness(-outlineWidth);

		target->draw(shape);
	}

	void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override
	{
		sf::ConvexShape shape(vertexCount);
		for (int32 x = 0; x < vertexCount; x++)
		{
			shape.setPoint(x, {vertices[x].x * meterToPixels, vertices[x].y * meterToPixels});
		}

		shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		shape.setOutlineColor(sf::Color::Transparent);

		target->draw(shape);
	}

	void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override
	{
		sf::CircleShape shape(radius * meterToPixels);

		shape.setOrigin({radius / 2.f, radius / 2.f});
		shape.setPosition(sf::Vector2f{center.x, center.y} * meterToPixels);

		shape.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		shape.setOutlineThickness(-outlineWidth);

		target->draw(shape);
	}

	void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override
	{
		sf::CircleShape shape(radius * meterToPixels);

		shape.setOrigin({radius / 2.f, radius / 2.f});
		shape.setPosition(sf::Vector2f{center.x, center.y} * meterToPixels);

		shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));

		target->draw(shape);
	}

	void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override
	{
		const auto line = (p2 - p1);

		sf::RectangleShape shape({outlineWidth, line.Length() * meterToPixels});
		shape.setOrigin({outlineWidth / 2.f, 0.f});
		shape.setRotation(sf::Vector2f{line.x, line.y}.angle());
		shape.setPosition(sf::Vector2f{p1.x, p1.y} * meterToPixels);

		shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));

		target->draw(shape);
	}

	void DrawTransform(const b2Transform &xf) override
	{
		auto const axisScale = 0.4f;

		DrawSegment(xf.p, xf.p + axisScale * xf.q.GetXAxis(), b2Color{1.0f, 0.0f, 0.0f});
		DrawSegment(xf.p, xf.p + axisScale * xf.q.GetYAxis(), b2Color{0.0f, 1.0f, 0.0f});
	}

	void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override
	{    
		DrawSolidCircle(p, size, {}, color);
	}
};