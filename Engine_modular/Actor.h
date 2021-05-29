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
	float getDamage();
	Vector2f getPosition();
	void setPosition(Vector2f);

protected:
	float health;
	float speed;
	float direction;
	float damage;
	Vector2f position;
};

