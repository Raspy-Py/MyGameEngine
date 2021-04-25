#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Configs.h"
#include <Windows.h>

using namespace sf;
using namespace std;


class Player
{
public:
	Player(float, float);
	~Player();

	float direction;
	float health;
	float speed;
	float rotationSpeed;
	Vector2f position;

	void listenKeyboard();
	void takeDamage(int);

private:
	void moveForward();
	void moveBackward();
	void turnLeft();
	void turnRight();
	void getInfo();
};

