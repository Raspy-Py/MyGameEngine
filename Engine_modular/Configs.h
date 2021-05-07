#pragma once
#include <string>

////////////////////////////////////////////////
// Місцезнаходження ресурсів гри              //
////////////////////////////////////////////////
const std::string PATH_LEVEL = "../data/levels/level.csv";
const std::string PATH_WALL_TEXTURE = "../data/textures/beacon.bmp";
const std::string PATH_DEFAULT_FONT = "../data/fonts/basic_font.ttf";

////////////////////////////////////////////////
// Параметри вікна                            //
////////////////////////////////////////////////
const float AR = 9.0 / 16;
const float WISHED_FPS = 60;
const int WIN_WIDTH = 1280;
const int WIN_HEIGHT = WIN_WIDTH * AR;
const int WIN_HALF_HEIGHT = WIN_HEIGHT / 2;
const int WIN_HALF_WIDTH = WIN_WIDTH / 2;
const int MINIMAP_SIZE = WIN_HALF_HEIGHT / 2;

////////////////////////////////////////////////
// Параметри ініціалізація гравця             //
////////////////////////////////////////////////
const int PLAYER_DEFAULT_HEALTH = 100;
const int PLAYER_DEFAULT_SPEED = 3; 
const float PLAYER_DEFAULT_ROTATION_SPEED = 0.07;

////////////////////////////////////////////////
// Математичні константи                      //
////////////////////////////////////////////////
const double PI = 3.141592653;
const double ANGLE_0 = 0;
const double ANGLE_90 = PI/2;
const double ANGLE_180 = PI;
const double ANGLE_270 = PI * 3 / 2;
const double ANGLE_360 = PI * 2;

////////////////////////////////////////////////
// Параметри лиття променів                   //
////////////////////////////////////////////////
const float FOV = 60.0 * PI / 180;              // Кут огляду
const float WTP = (WIN_WIDTH / 2) / tan(FOV/2); // Відстань від гравця до площини проектування зображення
const float ABR = FOV / WIN_WIDTH;              // Кут між сусідніми променями

////////////////////////////////////////////////
// Параметри текстур                          //
////////////////////////////////////////////////
const int TEXTURE_RES = 32;                     // Роздільна здатність текстур
const float WALL_HEIGHT = 32;