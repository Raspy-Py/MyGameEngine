#pragma once
#include <SFML/Graphics.hpp>
#include "Configs.h"
#include "Player.h"
#include "Map.h"

class RayCasting
{
public:
	RayCasting();
	~RayCasting();

	float* raysLength;
	int* raysPositionsOnWalls; // для текстурування
	float** raysEndCords;

	void castRays(Player&, Map&);

private:
	Vector2f infty;

	float normalizeAngle(float);
	float distBtwnPoints(float, float, float, float);
};