#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Configs.h"

using namespace sf;

class Button:public RectangleShape
{
public:
	Button(Vector2f position, Vector2f size, Color color = Color(200,200,200));
	~Button();

	enum status {
		DEFAULT,
		HOVER,
		ACTIVE
	};

	// Перевіряє, чи нажата кнопка
	bool isPressed(RenderWindow&);
	// Встановлює режим відображення кнопки
	void setMode(int);
	// Встановлює налаштування тексту за зразком
	void setTextBySample(Text&);
	// Встановлює розмір шрифту
	void setTextSize(int);
	// Встановлює Шрифт тексту
	void setTextFont(Font&);
	// Встановлює колір тексту
	void setTextColor(Color);
	// Встановлює напис на кнопці
	void setTextString(std::string);
	// Встановлює місцезнаходження тексу в локальний координатах кнопки 
	void setTextPosition(Vector2f);
	// Повертає параметри тексту
	Text& getTextSample();
	// Відображає кнопку на екрані
	void draw(RenderWindow&);
	// Вирівнює текст посередині кнопки
	void alignTextCentre(float);

private:
	Text text; // Текст на кпопці
	Color defaultColor; // Колір кнопки в НЕактивному режимі
	Color hoverColor; // Колір кнопки в активному режимі
	Color activeColor; // Колір нажатої кнопки

	// Нормалізує значення каналу кольору в межах [0, 255]
	unsigned int norm(int);
};