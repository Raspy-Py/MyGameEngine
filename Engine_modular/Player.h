#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include "Configs.h"
#include "FPSHandler.h"
#include "Map.h"
#include "Actor.h"

using namespace sf;
using namespace std;

class Map;

class Player : public Actor
{
public:
	Player(float, float);
	~Player();

	virtual void listenKeyboard(FPS&, Map&);
	void takeDamage(int);
	void rotateByMouse(FPS&, int);

private:
	int colBoxSize;
	float speedInStrafe;
	float rotationSpeed;
	float strafeSpeed;

	void moveForward(FPS&, Map&);
	void moveBackward(FPS&, Map&);
	void strafeRight(FPS&, Map&);
	void strafeLeft(FPS&, Map&);
	void turnLeft(FPS&);
	void turnRight(FPS&);
	void getInfo();
	bool isWallColided(float, float, Map&);
	int limitRotationSpeed(int);
};