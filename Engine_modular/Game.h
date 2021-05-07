#pragma once
#include "Configs.h"

using namespace sf;

class FPS;
class RayCasting;
class Player;
class Map;
class Render;

class Game
{
public:
	Game();
	~Game();

	void runGame();

	void mainMenu();
	void levelCreationMenu();
	void levelChoosingMenu();
	void gameCycle();
private:
	RenderWindow window;
	ContextSettings settings;
};
