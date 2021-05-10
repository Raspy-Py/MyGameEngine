#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"
#include "Render.h"
#include "FPSHandler.h"
#include "Button.h"

using namespace sf;

class Game
{
public:
	Game();
	~Game();

	void runGame();
	void startLevel();
	void mainMenu();
	void levelCreationMenu();
	void levelChooseMenu();

private:
	RenderWindow window;

	enum Mode {
		MAIN_MENU,
		LEVEL_CREATION_MENU,
		LEVEL_CHOOSE_MENU,
		START_LEVEL
	};

	int toDisplay;
	Font menuFont;
	Sprite background;
    Texture backgroundTexture;
};

