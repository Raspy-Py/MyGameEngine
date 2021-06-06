#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "EntitySprite.h"
#include "Monster.h"
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"
#include "Render.h"
#include "FPSHandler.h"
#include "Button.h"

using namespace sf;

class Game
{
public:
	Game();
	~Game();

	// ������� �������, �� ������� ���� ���
	void runGame();

private:
	////////////////////////////////////////////////
	// 	������ ������� ��������                  //
	////////////////////////////////////////////////

	// ������� �����
	void startLevel();
	// ������� ������� ����
	void mainMenu();
	// ������� ���� ��������� ��������������� ����
	void levelCreationMenu();
	// ������� ���� ������ ����
	void levelChooseMenu();
	// ������� ���� ���������� ���
	void endgameMenu();

	// ������ ����� �����������
	enum Mode {
		MAIN_MENU,
		LEVEL_CREATION_MENU,
		LEVEL_CHOOSE_MENU,
		START_LEVEL,
		END_GAME
	};
	int toDisplay; // ������� �����

	////////////////////////////////////////////////
	// 	������ ��'���� ���                       //
	////////////////////////////////////////////////

	RenderWindow window; // ³��� ��������
	Map map; // �����, �� �� ����������� �����
	Font menuFont; // �������� ����� � ��
	Text gameResultText; // ����������� ��� ��������� ���
	Sprite background; // ����� ��� ��� ����
    Texture backgroundTexture; // �������� �������� ���� ����
	RenderImage render; // ��'���, ������������ �� ������ ���������� �� ��� ���
	VertexArray playerHealthIndicator; // ��������� ������'� ������

	////////////////////////////////////////////////
	// 	��������� ����������� ��������� ����     //
	////////////////////////////////////////////////

	VertexArray levelPreview; // ����� ������ ����� ���� ��� ������������ ���������
	Sprite wallSpritePreview; // ������ ���� ���� ��� ������������ ���������
	Texture wallTexturePreview; // �������� ���� ���� ��� ������������ ���������
	RectangleShape floorPreview; // ϳ����� ���� ��� ������������ ���������
	RectangleShape ceilingPreview; // ����� ���� ��� ������������ ���������
	Text namePreview; // ����� ����
	int** levelPreviewPlan; // ����� ���� ��� ������������ ���������
	int levelPreviewSize; // ����� ���� ��� ������������ ���������

	Vector2f texturePreviewPos; // ̳�������������� ������� ���� �� �����
	Vector2f floorPreviewPos; // ̳�������������� ������ ������
	Vector2f ceilingPreviewPos; // ̳�������������� ������ ����
	Vector2f mapPreviewPos; // ̳�������������� ������ ������ ����� ����
	

	////////////////////////////////////////////////
	// 	��������� ����                           //
	////////////////////////////////////////////////
	std::vector<Color> floorColor; // ���� ������
	std::vector<Color> ceilingColor;  // ���� ����
	std::vector<std::string> levelName;  // ����� ����
	std::vector<std::string> levelPlanPath;  // ���� �� ����� ����
	std::vector<std::string> wallTexturePath; // ���� �� �������� ����

	// �������� ����� � ��'���� ����� 'Color'
	Color& stringToColor(std::string);
	// �������� ������ ���: ����������\��������� ���
	bool checkGameStatus(Player&, Monster&);
	// ������� ��������� ������'�
	void updateIndicator(float);
	// ��������� ������ ����
	void loadTemplates();
	// ������� � ���������� ���'�� �������
	void unloadTemplates();
	// �������� �� ��������� ������
	void switchTemplate(int);
};