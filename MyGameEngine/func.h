#pragma once
#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#define PI 3.14159
#define P2 PI/2
#define P3 3*PI/2

using namespace sf;
using namespace std;

const int WIN_WIDTH = 320;
const int WIN_HEIGHT = 200;
const int CELL_SIZE = 32; // Розмір одного блоку на мапі (стандартно 64) 
const double FOV = 60.0 * PI / 180;
const double WTP = WIN_WIDTH / (2 * tan(FOV * PI / 360)); // Відстань від гравця до площини проектування зображення
const double ABR = FOV / WIN_WIDTH; // Кут між сусідніми променями

class Player {

public:
    int rays_lenth[WIN_WIDTH];
    float health;
    double dir = 0;
    Vector2f end_cords[WIN_WIDTH];
    Vector2f cords;


    Player(double, double, double);
    void move_player(int);
    void rotate_player(int);
    void get_info();
    void cast_rays(int**, RenderWindow&);
    float dist_to_player(float, float);

private:
    double speed;
    double rotationSpeed = 5 * PI / 180;
};

template <typename T>
void print_matrix(T** arr, int n); // Функція для налагодження
template <typename T>
void print_vector(T*, int);

void start_game(); // Головна функція гри 
int** load_map_plan(); // Завантаження мапи з файлу в оперативну пам'ять
void draw_minimap(int**, VertexArray&); // Відобаражає мінікарту рівня
void init_rays(VertexArray &, Player &);
double normalize_angle(double);


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
//
//float rayByX, rayByY;
//float dy, dx;
//int Ax, Ay, Bx, By;
//bool foundWall;
//
//for (double a = dir - FOV / 2; a < dir + FOV / 2; a += ABR)
//{
//    dy = CELL_SIZE / sin(a);
//    dx = CELL_SIZE / cos(a);
//
//    if (a < PI / 2 && a > 0 || a >= PI * 3 / 4) Ax = (int(cords.x) / 64) * 64 + 64;
//    else Ax = (int(cords.x) / 64) * 64 - 1;
//    Ay = (Ax - cords.x) * tan(a) + cords.y;
//
//
//    if (a <= 180) By = (int(cords.y) / 64) * 64 + 64;
//    else By = (int(cords.y) / 64) * 64 - 1;
//    Bx = cords.x + (By - cords.y) / tan(a);
//
//    rayByX = (Ax - cords.x) / cos(a);
//    rayByY = (By - cords.y) / sin(a);
//
//    if (level[Ay / 64][Ax / 64] || level[By / 64][Bx / 64]) foundWall = true;
//    else foundWall = false;
//
//    while (!foundWall)
//    {
//
//    }
//}