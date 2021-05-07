#pragma once
#include <SFML/Graphics.hpp>
#include "configs.h"
#include <iostream>

using namespace sf;

class FPS
{
public:
	FPS();
	FPS(RenderWindow&);
	~FPS();

	void setFPSLimit(RenderWindow&);
	float getFPS();
	void updateFPS();
	void draw(RenderWindow&);

private:
	Clock time;
	Time elapsed;
	Text stringFPS;
	Font stringFPSFont;
	std::string stringFPSPrefix;

	float actualFPS;
	float wishedFPS;
	void initStringFPS();
};