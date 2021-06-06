#include "RayCasting.h"

RayCasting::RayCasting()
{
	////////////////////////////////////////////////
	// 	�������� ���'��� �� �� ������          //
	////////////////////////////////////////////////

	raysLength = new float[WIN_WIDTH];
	raysPositionsOnWalls = new int[WIN_WIDTH];
	raysEndCords = new float*[WIN_WIDTH];
	isWallHorizontal = new bool[WIN_WIDTH];

	for (int i = 0; i < WIN_WIDTH; i++)
	{
		raysEndCords[i] = new float[2];
	}

	// ���������� ������ ����������� ������� �������
	infty.x = 10000;
	infty.y = 10000;
}

RayCasting::~RayCasting()
{
	////////////////////////////////////////////////
	//  ������� �������� ������� ���'���       //
	////////////////////////////////////////////////

	/*delete [] raysLength;
	delete[] raysPositionsOnWalls;

	for (int i = 0; i < WIN_WIDTH; i++)
	{
		delete[] raysEndCords;
	}

	delete[] raysEndCords;*/
}

void RayCasting::castRays(Player& player, Map& map)
{
	////////////////////////////////////////////////
	// ��������� ������ �� �������� � ������   //
	// ���������� �� ����� � �������,            //
	// � ����� ����� �� ����� ��� �������������  //
	////////////////////////////////////////////////

	

	float x0 = player.getPosition().x; // ���������� ������
	float y0 = player.getPosition().y; // ���������� ������
	float xa = 0, ya = 0;         // ���������� ������� �������� ������� �� �����
	float dx = 0, dy = 0;         // ����
	float ra;                     // �������� �������� � �������
	float rau = player.getDirection() - FOV/2; // ��������������� ���
	float tanA;
	float hLen, vLen;             // ������� �������
	int xh, yh, xv, yv;           // ��������� ����� �������
	int iter = 1;                 // ����� �������� ����� ��������
	int quadSize = map.getCellSize();  // ����� ���� ������� 
	int mapSize = map.getLevelSize();  // ����� ���� � ��������
	int worldSize = mapSize * quadSize - 1; // ����� ���� 
	int** levelPlan = map.getLevelPlan(); // �������� �� ����� ����

	for (int rNumber = 0; rNumber < WIN_WIDTH; rNumber++, rau+=ABR)
	{
		ra = normalizeAngle(rau);

		//---������� � ������������� �����---//

		tanA = tan(ra);
		if (ra > ANGLE_270 || ra < ANGLE_90) // ������ �������� ������
		{
			xa = (int)x0 / quadSize * quadSize + quadSize;
			ya = (xa - x0) * tanA + y0;
			dx = quadSize;
			dy = quadSize * tanA;
		}
		else if (ra > ANGLE_90 && ra < ANGLE_270) // ������ �������� ����
		{
			xa = (int)x0 / quadSize * quadSize - 1;
			ya = (xa - x0) * tanA + y0;
			dy = -quadSize * tanA;
			dx = -quadSize;
		}
		else {
			iter = mapSize;
		}

		// ���������� �� �� �������� ������ �� ��� ����
		// �� �� ��������� �� ����
		if (xa >= worldSize || ya >= worldSize || xa < 0 || ya < 0) iter = mapSize;
		else if (levelPlan[int(ya) / quadSize][int(xa) / quadSize]) iter = mapSize;
		else iter = 1;

		while (iter < mapSize)
		{
			iter++;

			xa += dx;
			ya += dy;

			if (xa >= worldSize || ya >= worldSize || xa < 0 || ya < 0) break;
			if (levelPlan[int(ya) / quadSize][int(xa) / quadSize] ) break;
		}
		xv = xa; yv = ya; // �������� ���������� ���� �������

		//---������� � ��������������� �����---//

		tanA = tan(ANGLE_90 - ra);
		if (ra < ANGLE_180 && ra > 0) // ������ �������� ����
		{
			ya = (int)y0 / quadSize * quadSize + quadSize;
			xa = (ya - y0) * tanA + x0;
			dx = quadSize * tanA;
			dy = quadSize;
		}
		else if (ra > ANGLE_180 && ra < ANGLE_360) // ������ �������� �����
		{
			ya = (int)y0 / quadSize * quadSize - 1;
			xa = (ya - y0) * tanA + x0;
			dx = -quadSize * tanA;
			dy = -quadSize;
		}
		else {
			iter = mapSize;
		}

		// ���������� �� �� �������� ������ �� ��� ����
		// �� �� ��������� �� ����
		if (xa >= worldSize || ya >= worldSize || xa < 0 || ya < 0) iter = mapSize;
		else if (levelPlan[(int)ya / quadSize][(int)xa / quadSize]) iter = mapSize;
		else iter = 1;

		while (iter < mapSize)
		{
			iter++;

			xa += dx;
			ya += dy;

			if (xa >= worldSize || ya >= worldSize || xa < 0 || ya < 0) break;
			if (levelPlan[(int)ya / quadSize][(int)xa / quadSize]) break;
		}
		xh = xa; yh = ya; // �������� ���������� ���� �������


		////////////////////////////////////////////////
		// � ���� ������� ������� �������� �        //
		// �������� �� �������� ��� ��� �����     //
		// � ������                                   //
		////////////////////////////////////////////////

		hLen = distBtwnPoints(x0, y0, xh, yh);
		vLen = distBtwnPoints(x0, y0, xv, yv);

		if (hLen < vLen) {
			raysEndCords[rNumber][0] = xh;
			raysEndCords[rNumber][1] = yh;
			raysLength[rNumber] = hLen* cos(player.getDirection() - ra);
			raysPositionsOnWalls[rNumber] = (int)xh % TEXTURE_RES;
			isWallHorizontal[rNumber] = true;
		}
		else
		{
			raysEndCords[rNumber][0] = xv;
			raysEndCords[rNumber][1] = yv;
			raysLength[rNumber] = vLen * cos(player.getDirection() - ra);
			raysPositionsOnWalls[rNumber] = (int)yv % TEXTURE_RES;
			isWallHorizontal[rNumber] = false;
		}
	}
}

int* RayCasting::getRaysPositionsOnWalls()
{
	return raysPositionsOnWalls;
}

bool* RayCasting::checkWallHorizontal()
{
	return isWallHorizontal;
}

float* RayCasting::getRaysLength()
{
	return raysLength;
}

float** RayCasting::getRaysEndCords()
{
	return raysEndCords;
}

float RayCasting::normalizeAngle(float a)
{
	if (a > ANGLE_360) return a - ANGLE_360;
	if (a < ANGLE_0  ) return a + ANGLE_360;
	return a;
}

float RayCasting::distBtwnPoints(float x0, float y0, float x1, float y1)
{
	return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
}
