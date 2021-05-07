#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include "Configs.h"
#include "Player.h"

//using namespace std;
using namespace sf;

class Player;

class Map
{
public:
	Map(int, std::string);
	~Map();

	int **levelPlan;
	int levelSize;
	int cellSize;
	float minimapSize;
	float minimapCellSize;
	float minimapToMapRelation;

	void draw(RenderWindow&);
	virtual void updateMinimap(Player &, float**);

private:
	VertexArray walls;
	VertexArray rays;
	VertexArray playerDirPointer;
	std::vector<CircleShape> enemyMarkers;
	CircleShape playerMarker;
	RectangleShape minimapBackground;
};