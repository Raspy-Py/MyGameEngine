#include "FPSHandler.h"

FPS::FPS()
{
	wishedFPS = WISHED_FPS;
	actualFPS = 1;

	time.restart();

	initStringFPS();
}

FPS::FPS(RenderWindow& window)
{
	wishedFPS = WISHED_FPS;
	actualFPS = 1;

	window.setFramerateLimit(wishedFPS);

	time.restart();

	initStringFPS();
}

FPS::~FPS()
{
}

void FPS::setFPSLimit(RenderWindow& window) {
	window.setFramerateLimit(wishedFPS);
}

float FPS::getFPS()
{
	return wishedFPS / actualFPS;
}

void FPS::updateFPS()
{
	elapsed = time.getElapsedTime();
	actualFPS = 1000.0 / (elapsed.asMilliseconds());
	stringFPS.setString(stringFPSPrefix + std::to_string(int(actualFPS)));
	time.restart();
}

void FPS::draw(RenderWindow& window)
{
	window.draw(stringFPS);
}

void FPS::initStringFPS()
{
	if (!stringFPSFont.loadFromFile(PATH_DEFAULT_FONT))
	{
		std::cout << "Failed to load font!" << std::endl;
	}

	stringFPSPrefix = "";

	int stringFPSSize = stringFPSPrefix.size() * 16 + 48;

	stringFPS.setFont(stringFPSFont);
	stringFPS.setCharacterSize(20);
	stringFPS.setFillColor(Color(0, 204, 51));
	stringFPS.setPosition(Vector2f(WIN_WIDTH - stringFPSSize, 0));
}
