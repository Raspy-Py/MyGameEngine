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

	// ���������� ��� ������� �����
	void setFPSLimit(RenderWindow&);
	// ������� ��������� ������� FPS �� ��������
	float getFPS();
	// ������� �������� FPS
	void updateFPS();
	// ³������� �� ����� FPS
	void draw(RenderWindow&);

private:
	Clock time; // �������� ��������
	Clock timeFromUpdate; // �������� ��� ���������� ���� �� ���������� ���������
	Time elapsed; // ��� �� ������������ �����
	Time update; // ��� �� ������������ ���������

	Text stringFPS; // ���� ��� ����������� FPS
	Font stringFPSFont; // ����� ������� ��� ����������� FPS
	float stringFPSSize; // ����� ������� ��� ����������� FPS
	std::string stringFPSPrefix; // ������� ������� ��� ����������� FPS

	float actualFPS; // ĳ����� FPS
	float wishedFPS; // ������� FPS

	// ����������� ������ ��� ����������� FPS
	void initStringFPS();
};