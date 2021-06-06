#pragma once
#include <SFML/Graphics.hpp>
#include "Configs.h"
#include "Player.h"
#include "Map.h"

class Player;
class Map;

class RayCasting
{
public:
	RayCasting();
	~RayCasting();

	int* raysPositionsOnWalls; // для текстурування
	bool* isWallHorizontal; // Для освітлення (горизонатальні стіни затемняємо)
	float* raysLength;
	float** raysEndCords;

	void castRays(Player&, Map&);

private:
	Vector2f infty;

	float normalizeAngle(float);
	float distBtwnPoints(float, float, float, float);
};