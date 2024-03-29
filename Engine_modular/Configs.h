#pragma once

////////////////////////////////////////////////
// ̳�������������� ������� ���              //
////////////////////////////////////////////////
const std::string PATH_MM_BACKGROUND = "../data/pictures/main_menu_background.jpeg";
const std::string PATH_WALL_TEXTURE = "../data/textures/beacon.bmp";
const std::string PATH_DEFAULT_FONT = "../data/fonts/basic_font.ttf";
const std::string PATH_TEMPLATES = "../data/levels_templates.csv";
const std::string PATH_WEAK_MONSTER = "../data/sprites/monster.bmp";
const std::string PATH_STRONG_MONSTER = "../data/sprites/monster_1.bmp";

////////////////////////////////////////////////
// ��������� ����                            //
////////////////////////////////////////////////
const float FONT_AR = 0.735;
const float AR = 9.0 / 16;
const float WISHED_FPS = 60;
const int WIN_WIDTH = 1000 + 1;
const int WIN_HEIGHT = (WIN_WIDTH -1 ) * AR;
const int WIN_HALF_HEIGHT = WIN_HEIGHT / 2;
const int WIN_HALF_WIDTH = (WIN_WIDTH-1) / 2;
const int MINIMAP_SIZE = WIN_HEIGHT / 2.5;
const int MIN_ELAPSED = 200;

////////////////////////////////////////////////
// ��������� ������������ ������             //
////////////////////////////////////////////////
const int MAX_MOUSE_DELTA = 65;
const int PLAYER_COLISION_BOX_SIZE = 7;         // �������� ������� �����������
const float PLAYER_DEFAULT_HEALTH = 100;
const float PLAYER_DEFAULT_SPEED = 3;
const float PLAYER_DEFAULT_DAMAGE = 10;
const float STRAFE_SPEED = 1.5;
const float PLAYER_DEFAULT_ROTATION_SPEED = 0.05;
const float MOUSE_SENS = 0.002;
const float POINTER_RADIUS = 3;
const float MONSTER_ATACK_DISTANCE = 32;
const int NUMBER_OF_MONSTERS = 10;

////////////////////////////////////////////////
// ����������� ���������                      //
////////////////////////////////////////////////
const float PI = 3.141592653;
const float ANGLE_0 = 0;
const float ANGLE_90 = PI/2;
const float ANGLE_180 = PI;
const float ANGLE_270 = PI * 3 / 2;
const float ANGLE_360 = PI * 2;

////////////////////////////////////////////////
// ��������� ����� ��������                   //
////////////////////////////////////////////////
const float FOV = 70.0 * PI / 180;              // ��� ������
const float WTP = (WIN_WIDTH / 2) / tan(FOV/2); // ³������ �� ������ �� ������� ������������ ����������
const float ABR = FOV / WIN_WIDTH;              // ��� �� �������� ���������
const float SHADING = 0.75;                     // ���� ����������

////////////////////////////////////////////////
// ��������� �������                          //
////////////////////////////////////////////////
const int MONSTER_SPRITE_RES = 32;              // �������� ��������� ���������� �������
const int TEXTURE_RES = 32;                     // ����� �������� ����
const float WALL_HEIGHT = 32;                   // ������ ����
