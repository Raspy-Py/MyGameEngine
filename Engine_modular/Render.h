#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "Configs.h"
#include "RayCasting.h"

using namespace sf;

class RenderImage
{
public:
	RenderImage();
	~RenderImage();

	// Ініціалізує параметри зображення на екрані
	void init(std::string, Color, Color);
	// Оновлює зображення на екрані
	void updateImage(RayCasting &);
	// Відображає все на екрані
	void draw(RenderWindow&);
	// видаляє текстуру з оперативної пам'яті
	void deleteTexture();
private:
	VertexArray image; // Масив вершин проекції стін на екран
	RectangleShape floor; // Плитка підоги
	RectangleShape ceiling; // Плитка стелі
	std::vector<Color> textureColorMap; // Послідовно виписані пікселі текстури
	Color** textureCols; // Текстура
	Color** textureColsShaded; // Затемнена текстура

	// Затемнює колір для імітації освітлення
	Color fadeColor(Color&);
	// Переводить число із 16-ї e 10-у СЧ
	unsigned hexToDec(char);
};

