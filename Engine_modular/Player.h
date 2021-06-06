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
class Monster;

class Player : public Actor
{
public:
	Player(int, int);
	~Player();

	void listenKeyboard(FPS&, Map&, float*, Monster&);
	void takeDamage(float);
	void rotateByMouse(FPS&, int);
	void shoot(Monster&, float*);

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
	bool isWallColided(float, float, Map&);
	int limitRotationSpeed(int);
};