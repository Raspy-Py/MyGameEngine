#pragma once
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


class Monster : public Actor
{

public:
	Monster();
	~Monster();
	
	void makeStep(FPS&, Map&); // ������� ������ ���� ����
	std::vector<Monster*>& getMonstersList(); // ������� ������ ����� �������
	void painterSort(const Vector2f&); // ����� ����� ������� �� ���������� �� ������
private:
	static std::vector<Monster*> monstersList; // ����� ����� �������

	std::vector<Vector2i> pathToPlayer; // ������ �������, ����� �� ��������� ���� �� ������

	float sqrDistance(const Vector2f&, const Vector2f&); // ������� ������ �� �������

	std::vector<Vector2i> aStarPathFind(int**, int, const Vector2i&, const Vector2i&); // ����� ����� �� ������
	Node* chooseCurrentNode(std::vector<Node*>&); // ����� ����� �������� ����� ���, �� �� �������� �������� F
	int distance(Vector2i, Vector2i); // �������� ������� �� ���������
	void removeFromOpenAndClose(Node*, std::vector<Node*>&); // ������� �����
	bool isPointReached(Map&);
};
