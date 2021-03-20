#define PI 3.14159
#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

const int WIN_WIDTH = 320;
const int WIN_HEIGHT = 200;
const int CELL_SIZE = 64; // Розмір одного блоку на мапі (стандартно 64) 
const double FOV = 60 * PI / 180;
const double WTP = WIN_WIDTH / (2 * tan(FOV * PI / 360)); // Відстань від гравця до площини проектування зображення
const double ABR = 60.0 / 320 * PI / 180; // Кут між сусідніми променями


template <typename T>
void print_matrix(T** arr, int n); // Функція для налагодження
template <typename T>
void print_vector(T*, int);

void start_game(); // Головна функція гри 
int** load_map_plan(); // Завантаження мапи з файлу в оперативну пам'ять
void draw_minimap(int**, VertexArray&); // Відобаражає мінікарту рівня

class Player {

public:
    float health;
    double dir = 0;
    Vector2f cords;
    Player(double, double, double);
    void move_player(int);
    void rotate_player(int);
    void get_info();

private:
    double speed;
    double rotationSpeed = 5 * PI / 180;
};

//class Map
//{
//public:
//    int**
//	Map();
//	~Map();
//
//private:
//
//};
//
//Map::Map()
//{
//}
//
//Map::~Map()
//{
//}