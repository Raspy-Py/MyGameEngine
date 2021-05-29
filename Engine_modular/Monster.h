#pragma once
#include "Actor.h"
#include "FPSHandler.h"
#include "Map.h"

// Тип вузла графа
enum class NodeType {
	UNCHECKED,
	OPEN,
	CLOSED,
	WALL
}; 

// Структура вузла графа 
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
	
	void makeStep(FPS&, Map&); // Монстер робить один крок
	std::vector<Monster*>& getMonstersList(); // Повертає список живих монстрів
	void painterSort(const Vector2f&); // Сортує масив монстрів за віддаленістю від гравця
private:
	static std::vector<Monster*> monstersList; // Масив живих монстрів

	std::vector<Vector2i> pathToPlayer; // Вектор клітинок, через які проходить шлях до гравця

	float sqrDistance(const Vector2f&, const Vector2f&); // Квадрат відстані між точками

	std::vector<Vector2i> aStarPathFind(int**, int, const Vector2i&, const Vector2i&); // Пошук шляху до гравця
	Node* chooseCurrentNode(std::vector<Node*>&); // Обирає серед відкритих вузлів той, що має найменше значення F
	int distance(Vector2i, Vector2i); // одинична відстань між вершинами
	void removeFromOpenAndClose(Node*, std::vector<Node*>&); // Закриває вузол
	bool isPointReached(Map&);
};
