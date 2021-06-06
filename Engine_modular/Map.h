#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stack>
#include "Monster.h"
#include "Configs.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Player;
class Monster;

class Map
{
public:
	Map();
	~Map();

	// Завантажує карту
	void loadMap(int, string); 
	// Функція відображення карти на екрані
	void draw(RenderWindow&); 
	// Оновлює положення маркерів на мінікарті
	void updateMinimap(Player &,Monster&, float**); 
	// Генерує рівень заданого розміру
	void generateMap(int, int); 
	// Видаляє деяку тимчасову інформацю при переході у головне меню
	void clearInfo(); 
	// Повертає схему рівня
	int** getLevelPlan();
	// Повертає розмір рівня
	int getLevelSize();
	// Повертає розмір клітинки
	int getCellSize();

private:
	VertexArray walls; // Масив вершин стін на мінікарті
	VertexArray rays; // Масив  вершин променів на мінікарті
	VertexArray playerDirPointer; // Лінія, що вказує напрямок руху гравця
	vector<CircleShape> enemyMarkers; // Марке ворога на мінікарті
	CircleShape playerMarker; // Маркер гравця на мінікарті
	RectangleShape minimapBackground; // Задній фон мінікарти

	int **levelPlan; // Схема рівня
	int levelSize; // Розмір рівня
	int cellSize; // Розмір одної клітинки на карті
	float minimapSize; // Розмір мінікарти
	float minimapCellSize; // Розмір клітинки на мінікарті
	float minimapToMapRelation; // Відношееня розмірів карти до мінікарти

	// Генерує заготовку для рівня
	int** genEmptyField(int); 
	// Генерує лабіринт із заготовки
	void genMaze(int**, int); 
	// Вибиває деякі стіни в лабіринті
	void crashWalls(int**, int); 
	// Перевіряє, чи є у даної клітинки невідвідані сусідні клітинки
	bool unvisitedNeighbors(int**, int, Vector2i&);  
	// Перевіряє належність клітинки полю
	bool onField(int, int, int); 
	// Знаходить наступну клітинку для алгоритму генерації лабіринту
	Vector2i findNewCell(int**, int, Vector2i&); 
};