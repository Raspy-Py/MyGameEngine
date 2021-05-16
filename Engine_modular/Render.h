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

	void init(std::string, Color&, Color&);
	void updateImage(RayCasting &);
	void draw(RenderWindow&);
	void deleteTexture();
private:
	VertexArray image;
	RectangleShape floor;
	RectangleShape ceiling;
	std::vector<Color> textureColorMap; // Послідовно випасані пікселі текстури
	Color** textureCols; // Текстура
	Color** textureColsShaded; // Затемнена текстура

	Color fadeColor(Color&);
	unsigned hexToDec(char);
};

