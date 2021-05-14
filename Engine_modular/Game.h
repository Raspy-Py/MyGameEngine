#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
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

private:
	////////////////////////////////////////////////
	// 	Головні функції програми                  //
	////////////////////////////////////////////////
	void startLevel();
	void mainMenu();
	void levelCreationMenu();
	void levelChooseMenu();

	enum Mode {
		MAIN_MENU,
		LEVEL_CREATION_MENU,
		LEVEL_CHOOSE_MENU,
		START_LEVEL
	};
	int toDisplay;

	////////////////////////////////////////////////
	// 	Основні об'єкти гри                       //
	////////////////////////////////////////////////
	RenderWindow window;
	Map map;
	Font menuFont;
	Sprite background;
    Texture backgroundTexture;
	RenderImage render;

	////////////////////////////////////////////////
	// 	Параметри попередньго перегляду рівня     //
	////////////////////////////////////////////////
	VertexArray levelPreview;
	Sprite wallSpritePreview;
	Texture wallTexturePreview;
	RectangleShape floorPreview;
	RectangleShape ceilingPreview;
	Text namePreview;
	int** levelPreviewPlan;
	int levelPreviewSize;

	Vector2f texturePreviewPos;
	Vector2f floorPreviewPos;
	Vector2f ceilingPreviewPos;
	Vector2f mapPreviewPos;
	

	////////////////////////////////////////////////
	// 	Параметри рівня                           //
	////////////////////////////////////////////////
	std::vector<Color> floorColor;
	std::vector<Color> ceilingColor;
	std::vector<std::string> levelName;
	std::vector<std::string> levelPlanPath;
	std::vector<std::string> wallTexturePath;

	Color& stringToColor(std::string);
	void loadTemplates();
	void unloadTemplates();
	void switchTemplate(int);
};