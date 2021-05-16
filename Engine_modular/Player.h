#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Configs.h"
#include "FPSHandler.h"
#include "Map.h"

using namespace sf;
using namespace std;

class Map;

class Player
{
public:
	Player(float, float);
	~Player();

	int colBoxSize;
	float speedInStrafe;
	float direction;
	float health;
	float speed;
	float rotationSpeed;
	float strafeSpeed;
	Vector2f position;

	virtual void listenKeyboard(FPS&, Map&);
	void takeDamage(int);
	void rotateByMouse(FPS&, int);

private:
	void moveForward(FPS&, Map&);
	void moveBackward(FPS&, Map&);
	void strafeRigth(FPS&, Map&);
	void strafeLeft(FPS&, Map&);
	void turnLeft(FPS&);
	void turnRight(FPS&);
	void getInfo();
	bool isWallColided(float, float, Map&);
	int limitRotationSpeed(int);
};