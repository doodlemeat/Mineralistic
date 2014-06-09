#include <SFML\Graphics.hpp>
#include <iostream>

#include "Box2DWorldDraw.h"

Box2DWorldDraw::Box2DWorldDraw(sf::RenderWindow* window)
{
	m_window = window;
}

void Box2DWorldDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	sf::Vector2f center;
	for (int i = 0; i < vertexCount; i++)
	{
		//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
		sf::Vector2f transformedVec = PhysicsScale::physToGame(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(sf::Color::Transparent);
	polygon.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

	m_window->draw(polygon);
}
void Box2DWorldDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	sf::ConvexShape polygon(vertexCount);
	for (int i = 0; i < vertexCount; i++)
	{
		//polygon.setPoint(i, SFMLDraw::B2VecToSFVec(vertices[i]));
		sf::Vector2f transformedVec = PhysicsScale::physToGame(vertices[i]);
		polygon.setPoint(i, sf::Vector2f(std::floor(transformedVec.x), std::floor(transformedVec.y))); // flooring the coords to fix distorted lines on flat surfaces
	}																								   // they still show up though.. but less frequently
	polygon.setOutlineThickness(-1.f);
	polygon.setFillColor(Box2DWorldDraw::GLColorToSFML(color, 60));
	polygon.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

	m_window->draw(polygon);
}
void Box2DWorldDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	sf::CircleShape circle(PhysicsScale::physToGame(radius));
	circle.setOrigin(PhysicsScale::physToGame(radius), PhysicsScale::physToGame(radius));
	circle.setPosition(PhysicsScale::physToGame(center));
	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineThickness(-1.f);
	circle.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

	m_window->draw(circle);
}
void Box2DWorldDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	sf::CircleShape circle(PhysicsScale::physToGame(radius));
	circle.setOrigin(PhysicsScale::physToGame(radius), PhysicsScale::physToGame(radius));
	circle.setPosition(PhysicsScale::physToGame(center));
	circle.setFillColor(Box2DWorldDraw::GLColorToSFML(color, 60));
	circle.setOutlineThickness(1.f);
	circle.setOutlineColor(Box2DWorldDraw::GLColorToSFML(color));

	b2Vec2 endPoint = center + radius * axis;
	sf::Vertex line[2] =
	{
		sf::Vertex(PhysicsScale::physToGame(center), Box2DWorldDraw::GLColorToSFML(color)),
		sf::Vertex(PhysicsScale::physToGame(endPoint), Box2DWorldDraw::GLColorToSFML(color)),
	};

	m_window->draw(circle);
	m_window->draw(line, 2, sf::Lines);
}
void Box2DWorldDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	sf::Vertex line[] =
	{
		sf::Vertex(PhysicsScale::physToGame(p1), Box2DWorldDraw::GLColorToSFML(color)),
		sf::Vertex(PhysicsScale::physToGame(p2), Box2DWorldDraw::GLColorToSFML(color))
	};

	m_window->draw(line, 2, sf::Lines);
}
void Box2DWorldDraw::DrawTransform(const b2Transform& xf)
{
	float lineLength = 0.4;

	b2Vec2 xAxis = xf.p + lineLength * xf.q.GetXAxis();
	sf::Vertex redLine[] =
	{
		sf::Vertex(PhysicsScale::physToGame(xf.p), sf::Color::Red),
		sf::Vertex(PhysicsScale::physToGame(xAxis), sf::Color::Red)
	};

	b2Vec2 yAxis = xf.p + lineLength * xf.q.GetYAxis();
	sf::Vertex greenLine[] =
	{
		sf::Vertex(PhysicsScale::physToGame(xf.p), sf::Color::Green),
		sf::Vertex(PhysicsScale::physToGame(yAxis), sf::Color::Green)
	};

	m_window->draw(redLine, 2, sf::Lines);
	m_window->draw(greenLine, 2, sf::Lines);
}
