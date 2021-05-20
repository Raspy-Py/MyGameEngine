#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

float Actor::getHealth()
{
	return health;
}

float Actor::getSpeed()
{
	return speed;
}

float Actor::getDirection()
{
	return direction;
}

Vector2f Actor::getPosition()
{
	return position;
}

void Actor::setPosition(Vector2f newPosition)
{
	position = newPosition;
}
