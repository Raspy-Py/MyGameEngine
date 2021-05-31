#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

void Actor::setHealth(float _health)
{
	health = _health;
}

void Actor::setSpeed(float _speed)
{
	speed = _speed;
}

void Actor::setDirection(float _direction)
{
	direction = _direction;
}

void Actor::setDamage(float _damage)
{
	damage = _damage;
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

float Actor::getDamage()
{
	return damage;
}

Vector2f Actor::getPosition()
{
	return position;
}

void Actor::setPosition(Vector2f newPosition)
{
	position = newPosition;
}
