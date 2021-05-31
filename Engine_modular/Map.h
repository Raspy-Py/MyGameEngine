#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stack>
#include "Monster.h"
#include "Configs.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Player;
class Monster;

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
	void updateMinimap(Player &,Monster&, float**);
	void generateMap(int, int);
	void clearInfo(); // Видаляє деяку інформацю при переході у головне меню

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