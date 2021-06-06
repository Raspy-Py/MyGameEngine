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

	// Встановити ліміт кількості кадрів
	void setFPSLimit(RenderWindow&);
	// Повертає відношення дійсного FPS до бажаного
	float getFPS();
	// Оновлює значення FPS
	void updateFPS();
	// Відображає на екрані FPS
	void draw(RenderWindow&);

private:
	Clock time; // Основний годинник
	Clock timeFromUpdate; // Годинник для замірювання часу від останнього оновлення
	Time elapsed; // Час від попередьного кадру
	Time update; // Час від попередьного оновлення

	Text stringFPS; // Текс для відображення FPS
	Font stringFPSFont; // Шрифт тексуту для відображення FPS
	float stringFPSSize; // Розмір тексуту для відображення FPS
	std::string stringFPSPrefix; // Префікс тексуту для відображення FPS

	float actualFPS; // Дійсний FPS
	float wishedFPS; // Бажаний FPS

	// Ініціалізація тексту для відображення FPS
	void initStringFPS();
};