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

	// ��������, �� ������ ������
	bool isPressed(RenderWindow&);
	// ���������� ����� ����������� ������
	void setMode(int);
	// ���������� ������������ ������ �� �������
	void setTextBySample(Text&);
	// ���������� ����� ������
	void setTextSize(int);
	// ���������� ����� ������
	void setTextFont(Font&);
	// ���������� ���� ������
	void setTextColor(Color);
	// ���������� ����� �� ������
	void setTextString(std::string);
	// ���������� ��������������� ����� � ��������� ����������� ������ 
	void setTextPosition(Vector2f);
	// ������� ��������� ������
	Text& getTextSample();
	// ³������� ������ �� �����
	void draw(RenderWindow&);
	// ������� ����� ��������� ������
	void alignTextCentre(float);

private:
	Text text; // ����� �� ������
	Color defaultColor; // ���� ������ � ����������� �����
	Color hoverColor; // ���� ������ � ��������� �����
	Color activeColor; // ���� ������ ������

	// �������� �������� ������ ������� � ����� [0, 255]
	unsigned int norm(int);
};