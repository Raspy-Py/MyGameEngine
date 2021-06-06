#include "FPSHandler.h"

FPS::FPS()
{
	wishedFPS = WISHED_FPS;
	actualFPS = 1;

	time.restart();
	timeFromUpdate.restart();

	initStringFPS();
}

FPS::FPS(RenderWindow& window)
{
	wishedFPS = WISHED_FPS;
	actualFPS = 1;

	window.setFramerateLimit(wishedFPS);

	time.restart();
	timeFromUpdate.restart();

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
	update = timeFromUpdate.getElapsedTime();
	actualFPS = 1000000.0 / (elapsed.asMicroseconds());

	if (update.asMilliseconds() > 200)
	{
	stringFPS.setString(stringFPSPrefix + std::to_string(int(actualFPS)));
	stringFPSSize = (stringFPSPrefix.size() + stringFPS.getString().getSize()) * stringFPS.getCharacterSize() * 0.85 + 2 + float(WIN_WIDTH)/32;
	stringFPS.setPosition(Vector2f(WIN_WIDTH - stringFPSSize, 0));

	timeFromUpdate.restart();
	}

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

	stringFPSSize = stringFPSPrefix.size() * stringFPS.getCharacterSize();

	stringFPS.setFont(stringFPSFont);
	stringFPS.setCharacterSize(20);
	stringFPS.setFillColor(Color(0, 204, 51));
	stringFPS.setPosition(Vector2f(WIN_WIDTH - stringFPSSize, 0));
}
