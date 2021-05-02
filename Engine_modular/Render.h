#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "RGBMap.h"
#include "Configs.h"

using namespace sf;
using namespace std;

class RenderImage
{
public:
	RenderImage(string);
	~RenderImage();

	void init();

private:
	VertexArray image;
	vector<Color> textureColorMap;
	Color** textureCols;
	unsigned hexToDec(char);
};

