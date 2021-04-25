#pragma once
#include <string>

////////////////////////////////////////////////
// Параметри вікна                            //
////////////////////////////////////////////////
const int WIN_WIDTH = 1600;
const int WIN_HEIGHT = 900;
const int MINIMAP_SIZE = 160;

////////////////////////////////////////////////
// Параметри ініціалізація гравця             //
////////////////////////////////////////////////
const int PLAYER_DEFAULT_HEALTH = 100;
const int PLAYER_DEFAULT_SPEED = 3; 
const float PLAYER_DEFAULT_ROTATION_SPEED = 0.1;

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