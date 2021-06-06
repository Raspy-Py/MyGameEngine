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

	// Головна функція, що запускає вссю гру
	void runGame();

private:
	////////////////////////////////////////////////
	// 	Головні функції програми                  //
	////////////////////////////////////////////////

	// Запускає рівень
	void startLevel();
	// Запускає головне меню
	void mainMenu();
	// Запускає меню генерації користувацького рівня
	void levelCreationMenu();
	// Запускає меню вибору рівня
	void levelChooseMenu();
	// Запускає меню завершення гри
	void endgameMenu();

	// Перелік режим відображення
	enum Mode {
		MAIN_MENU,
		LEVEL_CREATION_MENU,
		LEVEL_CHOOSE_MENU,
		START_LEVEL,
		END_GAME
	};
	int toDisplay; // Обраний режим

	////////////////////////////////////////////////
	// 	Основні об'єкти гри                       //
	////////////////////////////////////////////////

	RenderWindow window; // Вікно програми
	Map map; // Карта, на які запускається рівень
	Font menuFont; // Головний шрифт у грі
	Text gameResultText; // Повідомлення при завершенні гри
	Sprite background; // Задній фон для меню
    Texture backgroundTexture; // Текстура заднього фону меню
	RenderImage render; // Об'єкт, відповідальний за рендер зображення під час гри
	VertexArray playerHealthIndicator; // Індикатор здоров'я гравця

	////////////////////////////////////////////////
	// 	Параметри попередньго перегляду рівня     //
	////////////////////////////////////////////////

	VertexArray levelPreview; // Масив вершин схеми рівня для попереднього перегляду
	Sprite wallSpritePreview; // Спрайт стіни рівня для попереднього перегляду
	Texture wallTexturePreview; // Текстура стіни рівня для попереднього перегляду
	RectangleShape floorPreview; // Підлога рівня для попереднього перегляду
	RectangleShape ceilingPreview; // Стеля рівня для попереднього перегляду
	Text namePreview; // Назва рівня
	int** levelPreviewPlan; // Схема рівня для попереднього перегляду
	int levelPreviewSize; // Розмір рівня для попереднього перегляду

	Vector2f texturePreviewPos; // Місцезнаходження спрайта стіни не екрані
	Vector2f floorPreviewPos; // Місцезнаходження плитки підлоги
	Vector2f ceilingPreviewPos; // Місцезнаходження плитки стелі
	Vector2f mapPreviewPos; // Місцезнаходження мачиву вершин схеми рівня
	

	////////////////////////////////////////////////
	// 	Параметри рівня                           //
	////////////////////////////////////////////////
	std::vector<Color> floorColor; // Колір підлоги
	std::vector<Color> ceilingColor;  // Колір стелі
	std::vector<std::string> levelName;  // Назва рівня
	std::vector<std::string> levelPlanPath;  // Шлях до схеми рівня
	std::vector<std::string> wallTexturePath; // Шлях до текстури стіни

	// Конвертує рядок в об'єкти класу 'Color'
	Color& stringToColor(std::string);
	// Перевіряє статус гри: продовжити\завершити гру
	bool checkGameStatus(Player&, Monster&);
	// Оновлює індикатор здоров'я
	void updateIndicator(float);
	// Завантажує шаблон рівня
	void loadTemplates();
	// Видаляє з оперативної пам'яті шаблони
	void unloadTemplates();
	// Перемикає на наступний шаблон
	void switchTemplate(int);
};