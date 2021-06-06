#pragma once
#include "EntitySprite.h"
#include "Actor.h"
#include "FPSHandler.h"
#include "Map.h"

// ��� ����� �����
enum class NodeType {
	UNCHECKED,
	OPEN,
	CLOSED,
	WALL
}; 

// ��������� ����� ����� 
struct Node {
	int g;
	int h;
	int f;
	Vector2i gridPos;
	Node* parent;
	NodeType type;
};

class Player;
class Map;
class EntitySprite;

class Monster : public Actor
{

public:
	Monster();
	~Monster();
	
	// ������� ����� ������� �� ��������� ��������� ��
	void checkStatus(Player&, Map&, EntitySprite&); 
	// ������� ������ ���� ����
	void makeStep(FPS&, Map&, Player&); 
	// ������� ������ �������
	void respawn(Map&, const Vector2f&, EntitySprite&); 
	// ������� ������� ������������, �� ����������
	int getMonstersLeft();
	
private:
	std::vector<Vector2i> pathToPlayer; // ������ �������, ����� �� ��������� ���� �� ������
	Vector2i playerLastPosition; // ������� ������ �� ��������� ��������
	static int monsterLeft; // ʳ������ ������������, �� ����������

	std::vector<Vector2i> aStarPathFind(int**, int, const Vector2i&, const Vector2i&); // ����� ����� �� ������
	Node* chooseCurrentNode(std::vector<Node*>&); // ����� ����� �������� ����� ���, �� �� �������� �������� F
	int distance(Vector2i, Vector2i); // �������� ������� �� ���������
	void removeFromOpenAndClose(Node*, std::vector<Node*>&); // ������� �����
	bool isPointReached(Map&); // ��������, �� �������� ����������� ������� ����� ��������
	float sqrDistance(const Vector2f&, const Vector2f&); // ������� ������ �� �������
};
