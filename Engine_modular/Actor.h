#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Actor
{
public:
	Actor();
	~Actor();

	float getHealth();
	float getSpeed();
	float getDirection();
	Vector2f getPosition();
	void setPosition(Vector2f);

protected:
	float health;
	float speed;
	float direction;
	Vector2f position;
};

