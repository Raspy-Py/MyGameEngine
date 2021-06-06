#pragma once
#include "EntitySprite.h"
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

class Player;
class Map;
class EntitySprite;

class Monster : public Actor
{

public:
	Monster();
	~Monster();
	
	static int monsterLeft;

	void checkStatus(Player&, Map&, EntitySprite&); // Первірка стану монстра та виконання відповідних дій
	void makeStep(FPS&, Map&, Player&); // Монстер робить один крок
	void respawn(Map&, const Vector2f&, EntitySprite&); // Створює нового монстра
	
private:
	std::vector<Vector2i> pathToPlayer; // Вектор клітинок, через які проходить шлях до гравця
	Vector2i playerLastPosition; // Позиція гравця на попередній ітерації
	float sqrDistance(const Vector2f&, const Vector2f&); // Квадрат відстані між точками

	std::vector<Vector2i> aStarPathFind(int**, int, const Vector2i&, const Vector2i&); // Пошук шляху до гравця
	Node* chooseCurrentNode(std::vector<Node*>&); // Обирає серед відкритих вузлів той, що має найменше значення F
	int distance(Vector2i, Vector2i); // одинична відстань між вершинами
	void removeFromOpenAndClose(Node*, std::vector<Node*>&); // Закриває вузол
	bool isPointReached(Map&);
};
