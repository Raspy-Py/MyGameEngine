#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stack>
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
	void generateMap(int, int);

private:
	VertexArray walls;
	VertexArray rays;
	VertexArray playerDirPointer;
	vector<CircleShape> enemyMarkers;
	CircleShape playerMarker;
	RectangleShape minimapBackground;

	
	int** genEmptyField(int);
	void genMaze(int**, int);
	void crashWalls(int**, int);
	bool unvisitedNeighbors(int**, int, Vector2i&);
	bool onField(int, int, int);
	Vector2i findNewCell(int**, int, Vector2i&);
};