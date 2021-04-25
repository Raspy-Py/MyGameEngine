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

const int WIN_WIDTH =   800;
const int WIN_HEIGHT = 512;
const double CELL_SIZE = 32; // Розмір одного блоку на мапі (стандартно 64) 
const double FOV = 60.0 * PI / 180;
const double WTP = 277; // Відстань від гравця до площини проектування зображення
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
    double dist_to_player(double, double);

private:
    double speed;
    double rotationSpeed = 5 * PI / 180;
};

template <typename T>
void print_matrix(T** arr, int n); // Функція для налагодження
template <typename T>
void print_vector(T*, int);

void draw_image(int*, VertexArray&);
void start_game(); // Головна функція гри 
int** load_map_plan(); // Завантаження мапи з файлу в оперативну пам'ять
void draw_minimap(int**, VertexArray&); // Відобаражає мінікарту рівня
void init_rays(VertexArray &, Player &);
double normalize_angle(double);