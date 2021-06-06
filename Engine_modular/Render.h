#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <vector>
#include <iostream>
#include "Configs.h"
#include "RayCasting.h"

using namespace sf;

class RenderImage
{
public:
	RenderImage();
	~RenderImage();

	// �������� ��������� ���������� �� �����
	void init(std::string, Color, Color);
	// ������� ���������� �� �����
	void updateImage(RayCasting &);
	// ³������� ��� �� �����
	void draw(RenderWindow&);
	// ������� �������� � ���������� ���'��
	void deleteTexture();
private:
	VertexArray image; // ����� ������ �������� ��� �� �����
	RectangleShape floor; // ������ �����
	RectangleShape ceiling; // ������ ����
	std::vector<Color> textureColorMap; // ��������� ������� ����� ��������
	Color** textureCols; // ��������
	Color** textureColsShaded; // ��������� ��������

	// �������� ���� ��� ������� ���������
	Color fadeColor(Color&);
	// ���������� ����� �� 16-� e 10-� ��
	unsigned hexToDec(char);
};

