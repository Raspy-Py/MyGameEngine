#pragma once
#include <SFML/Graphics.hpp>
#include "Configs.h"
#include "Player.h"
#include "Map.h"

class Player;
class Map;

class RayCasting
{
public:
	RayCasting();
	~RayCasting();


	// ������ ������
	void castRays(Player&, Map&);

	// ������� ���������� ������ �� ���
	int* getRaysPositionsOnWalls(); 
	// ��������, �� � ���� �������������� (��� ���������)
	bool* checkWallHorizontal(); 
	// ������� ������� ��� ������� ������� �� �����
	float* getRaysLength(); 
	// ���������� ����� �������
	float** getRaysEndCords(); 

private:
	Vector2f infty; // ������ ������ �������
	int* raysPositionsOnWalls; // ���������� ������ �� ��� (��� ���������� �������)
	bool* isWallHorizontal; // �� � ���� �������������� (��� ���������)
	float* raysLength; // ������� ������� ��� ������� ������� �� �����
	float** raysEndCords; // ���������� ����� �������

	// �������� ��� � ����� [0, 359]
	float normalizeAngle(float);
	// ������� ������� �� ����� �������
	float distBtwnPoints(float, float, float, float);
};