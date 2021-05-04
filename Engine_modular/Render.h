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
	RenderImage(string);
	~RenderImage();

	void init();
	void updateImage(RayCasting &);
	void draw(RenderWindow&);

private:
	VertexArray image;
	VertexArray floor;
	VertexArray ceiling;
	vector<Color> textureColorMap;
	Color** textureCols;

	unsigned hexToDec(char);
};

