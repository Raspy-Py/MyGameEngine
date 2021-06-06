#pragma once
#include <fstream>
#include "Configs.h"
#include "SFML/Graphics.hpp"
#include "RayCasting.h"
#include "Player.h"

class RayCasting;
class Player;
class Map;

class EntitySprite
{
public:
	EntitySprite();
	~EntitySprite();

	// ³������� ������ �� �����
	void draw(RenderWindow&, RayCasting&);
	// ��������� ������ � �����
	void loadSprite(std::string, int);
	// ������� ������
	void calculateSprite(Player&, Vector2f);
private:
	Color** spriteTex; // ����������� ������ ��������
	std::vector<VertexArray> actualSprite; // ������������ ������
	double distToPlayer; // ³������ �� ������
	bool shouldBeDisplayed; // ���������, �� ������, �� ����� ���������� ������
	int spriteLeftFirstCol; // ������� �������
	int leftSideColOnDisplay; // ������� ������� �� �����

	// ������� ��� �������� �������
	double vectorToAngle(Vector2f);
	// ��������, �� ��������� ��'��� � ��� ����
	bool isInFieldOfView(float, Vector2f);
	// ����������� � 16-� ������� � 10-�
	unsigned hexToDec(char);
};


