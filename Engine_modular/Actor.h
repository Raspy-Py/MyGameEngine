#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Actor
{
public:
	Actor();
	~Actor();

	// Встановити рівень пунктів здоров'я
	void setHealth(float); 
	// Встановити швидкість
	void setSpeed(float);
	// Встановити напрямок руху
	void setDirection(float);
	// Встановити шкоду, яку завдаватиме сутність
	void setDamage(float);
	// Встановити місце знаходження
	void setPosition(Vector2f);
	// Повертає рівень пунктів здоров'я
	float getHealth();
	// Повертає швидкість
	float getSpeed();
	// Повертає напрямок руху
	float getDirection();
	// Повертає шкоду, яку завдаватиме сутність
	float getDamage();
	// Повертає місце знаходження
	Vector2f getPosition();

protected:
	float health; // К-ть пунктів доров'я
	float speed; // Швидкість руху
	float direction; // Напрямок руху
	float damage; // Шкода, яку завдаватиме сутність
	Vector2f position; // Місце знаходження
};

