#pragma once
#include <fstream>
#include "Configs.h"


using namespace sf;

class RayCasting;

class RenderImage
{
public:
	RenderImage(std::string);
	~RenderImage();

	void init();
	void updateImage(RayCasting &);
	void draw(RenderWindow&);

private:
	VertexArray image;
	VertexArray floor;
	VertexArray ceiling;
	std::vector<Color> textureColorMap;
	Color** textureCols;

	unsigned hexToDec(char);
};

