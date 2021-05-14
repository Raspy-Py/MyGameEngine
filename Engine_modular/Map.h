#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Configs.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Player;

class Map
{
public:
	Map();
	~Map();

	int **levelPlan;
	int levelSize;
	int cellSize;
	float minimapSize;
	float minimapCellSize;
	float minimapToMapRelation;

	void loadMap(int, string);
	void draw(RenderWindow&);
	void updateMinimap(Player &, float**);

private:
	VertexArray walls;
	VertexArray rays;
	VertexArray playerDirPointer;
	vector<CircleShape> enemyMarkers;
	CircleShape playerMarker;
	RectangleShape minimapBackground;
};