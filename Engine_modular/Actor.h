#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Actor
{
public:
	Actor();
	~Actor();

	void setHealth(float);
	void setSpeed(float);
	void setDirection(float);
	void setDamage(float);
	void setPosition(Vector2f);
	float getHealth();
	float getSpeed();
	float getDirection();
	float getDamage();
	Vector2f getPosition();

protected:
	float health;
	float speed;
	float direction;
	float damage;
	Vector2f position;
};

