#pragma once
#include <fstream>
#include "Configs.h"
#include "SFML/Graphics.hpp"
#include "RayCasting.h"
#include "Player.h"

class RayCasting;
class Player;
class Map;

class EntitySprite
{
public:
	EntitySprite();
	~EntitySprite();

	// Відображує спрайт на екрані
	void draw(RenderWindow&, RayCasting&);
	// Завантажує спрайт з файла
	void loadSprite(std::string, int);
	// Маштабує спрайт
	void calculateSprite(Player&, Vector2f);
private:
	Color** spriteTex; // Послідовність пікселів текстури
	std::vector<VertexArray> actualSprite; // Маштабований спрайт
	double distToPlayer; // Відстань до гравця
	bool shouldBeDisplayed; // Прапорець, що показує, чи треба відображати спрайт
	int spriteLeftFirstCol; // Абсциса спрайта
	int leftSideColOnDisplay; // Абсциса спрайта на екрані

	// Повертає кут напрямку вектора
	double vectorToAngle(Vector2f);
	// Перевіряє, чи знаходить об'єкт в полі зору
	bool isInFieldOfView(float, Vector2f);
	// Переведення з 16-ї системи в 10-у
	unsigned hexToDec(char);
};


