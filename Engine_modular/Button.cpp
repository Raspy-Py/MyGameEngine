#pragma once
#include "Button.h"

Button::Button(Vector2f position, Vector2f size, Color color)
{
	setPosition(position);
	setSize(size);
	setFillColor(color);

	defaultColor = color;

	text.setPosition(Vector2f(position.x + 10, position.y));

	hoverColor.r = norm(color.r + 20);
	hoverColor.g = norm(color.g + 20);
	hoverColor.b = norm(color.b + 20);
	hoverColor.a = norm(color.a + 20);

	activeColor.r = norm(color.r - 15);
	activeColor.g = norm(color.g - 15);
	activeColor.b = norm(color.b - 15);
	activeColor.a = norm(color.a + 40);
}

Button::~Button()
{
}

Text& Button::getTextSample()
{
	return text;
}

bool Button::isPressed(RenderWindow& window)
{
	Vector2i mousePosition = Mouse::getPosition(window);

	if (mousePosition.x >= getPosition().x &&
		mousePosition.y >= getPosition().y &&
		mousePosition.x <= getPosition().x + getSize().x &&
		mousePosition.y <= getPosition().y + getSize().y)

		if (Mouse::isButtonPressed(Mouse::Left)) {
			setMode(status::ACTIVE);
			return true;
		}else
			setMode(status::HOVER);
	else
		setMode(status::DEFAULT);

	return false;
}

void Button::setMode(int st) {
	switch (st)
	{
	case DEFAULT:
		setFillColor(defaultColor);
		break;
	case HOVER:
		setFillColor(hoverColor);
		break;
	case ACTIVE:
		setFillColor(activeColor);
		break;
	default:
		std::cout << "Wrong button status (" << std::string(text.getString()) << ")" <<std::endl;
		break;
	}
}

void Button::setTextBySample(Text& tSample)
{
	text = tSample;
}

void Button::setTextSize(int tSize)
{
	text.setCharacterSize(tSize);
}

void Button::setTextFont(Font& tFont)
{
	text.setFont(tFont);
}

void Button::setTextColor(Color tColor)
{
	text.setFillColor(tColor);
}

void Button::setTextString(std::string tString)
{
	text.setString(tString);
}

void Button::setTextPosition(Vector2f tPosition)
{
	text.setPosition(tPosition);
}

void Button::draw(RenderWindow& window)
{
	window.draw(*this);
	window.draw(text);
}

void Button::alignTextCentre(float fontAR)
{
	float textX;
	float textY;

	textY = this->getPosition().y + (this->getSize().y - text.getCharacterSize()) / 2;
	textX = this->getPosition().x + (this->getSize().x - text.getCharacterSize() * text.getString().getSize()*fontAR) / 2;

	text.setPosition(textX, textY);
}

unsigned int Button::norm(int n)
{
	if (n >= 0 && n <= 255)
		return n;
	if (n > 255)
		return 255;
	return 0;
}
