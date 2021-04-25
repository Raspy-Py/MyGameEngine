#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Configs.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Map
{
public:
	Map(int, string);
	~Map();

	int **levelPlan;
	int levelSize;
	int cellSize;
	float minimapSize;
	float minimapCellSize;
	float minimapToMapRelation;

	void draw(RenderWindow&);
	void updateMinimap(Player, float**);

private:
	VertexArray walls;
	VertexArray rays;
	VertexArray playerDirPointer;
	vector<CircleShape> enemyMarkers;
	CircleShape playerMarker;
	RectangleShape minimapBackground;
};