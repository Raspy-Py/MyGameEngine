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


	// Кинути промені
	void castRays(Player&, Map&);

	// Повертає координати стовця на стіні
	int* getRaysPositionsOnWalls(); 
	// перевіряє, чи є стіна горизонатільною (для освітлення)
	bool* checkWallHorizontal(); 
	// Довжини променів для кодного стовпця на екрані
	float* getRaysLength(); 
	// Координати кінців променів
	float** getRaysEndCords(); 

private:
	Vector2f infty; // Вектор великої довжини
	int* raysPositionsOnWalls; // Координата стовця на стіні (для накладання текстур)
	bool* isWallHorizontal; // Чи є стіна горизонатільною (для освітлення)
	float* raysLength; // Довжини променів для кодного стовпця на екрані
	float** raysEndCords; // Координати кінців променів

	// Нормалізує кут в межах [0, 359]
	float normalizeAngle(float);
	// Повертає відстань між двома точками
	float distBtwnPoints(float, float, float, float);
};