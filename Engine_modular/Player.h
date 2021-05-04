#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Configs.h"
#include "FPSHandler.h"

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

	void listenKeyboard(FPS &);
	void takeDamage(int);

private:
	void moveForward(FPS&);
	void moveBackward(FPS&);
	void turnLeft(FPS&);
	void turnRight(FPS&);
	void getInfo();
};