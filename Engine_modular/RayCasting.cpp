#include "RayCasting.h"

RayCasting::RayCasting()
{
	////////////////////////////////////////////////
	// 	Виділяємо пам'ять під усі масиви          //
	////////////////////////////////////////////////

	raysLength = new float[WIN_WIDTH];
	raysPositionsOnWalls = new int[WIN_WIDTH];
	raysEndCords = new float*[WIN_WIDTH];
	isWallHorizontal = new bool[WIN_WIDTH];

	for (int i = 0; i < WIN_WIDTH; i++)
	{
		raysEndCords[i] = new float[2];
	}

	// Ініціалізуємо вектор максимальної довжини променя
	infty.x = 10000;
	infty.y = 10000;
}

RayCasting::~RayCasting()
{
	////////////////////////////////////////////////
	//  Очищаємо динамічно виділену пам'ять       //
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
	// Випускаємо промені та записуємо в масиви   //
	// координати їх кінців і довжини,            //
	// а також точки на стінах для тесктуризації  //
	////////////////////////////////////////////////

	

	float x0 = player.getPosition().x; // Координата гравця
	float y0 = player.getPosition().y; // Координата гравця
	float xa = 0, ya = 0;         // Координати першого перетину променя із сіткою
	float dx = 0, dy = 0;         // Крок
	float ra;                     // Напрямок промення в радіанах
	float rau = player.getDirection() - FOV/2; // Ненормалізований кут
	float tanA;
	float hLen, vLen;             // Довжини променів
	int xh, yh, xv, yv;           // Кординати кінців променів
	int iter = 1;                 // Номер ітерації лиття промення
	int quadSize = map.getCellSize();  // Розмір однієї клітинки 
	int mapSize = map.getLevelSize();  // Розмір рівня в клітинках
	int worldSize = mapSize * quadSize - 1; // Розмір рівня 
	int** levelPlan = map.getLevelPlan(); // Покажчик на сіхему рівня

	for (int rNumber = 0; rNumber < WIN_WIDTH; rNumber++, rau+=ABR)
	{
		ra = normalizeAngle(rau);

		//---Перетин з вертикальними лініями---//

		tanA = tan(ra);
		if (ra > ANGLE_270 || ra < ANGLE_90) // Промінь випущено вправо
		{
			xa = (int)x0 / quadSize * quadSize + quadSize;
			ya = (xa - x0) * tanA + y0;
			dx = quadSize;
			dy = quadSize * tanA;
		}
		else if (ra > ANGLE_90 && ra < ANGLE_270) // Промінь випущено вліво
		{
			xa = (int)x0 / quadSize * quadSize - 1;
			ya = (xa - x0) * tanA + y0;
			dy = -quadSize * tanA;
			dx = -quadSize;
		}
		else {
			iter = mapSize;
		}

		// Перевіряємо чи не виходить промінь за межі рівня
		// та чи натраплює на стіну
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
		xv = xa; yv = ya; // Збергіаємо координати кінця променя

		//---Перетин з горизонтальними лініями---//

		tanA = tan(ANGLE_90 - ra);
		if (ra < ANGLE_180 && ra > 0) // Промінь випущено вниз
		{
			ya = (int)y0 / quadSize * quadSize + quadSize;
			xa = (ya - y0) * tanA + x0;
			dx = quadSize * tanA;
			dy = quadSize;
		}
		else if (ra > ANGLE_180 && ra < ANGLE_360) // Промінь випущено вверх
		{
			ya = (int)y0 / quadSize * quadSize - 1;
			xa = (ya - y0) * tanA + x0;
			dx = -quadSize * tanA;
			dy = -quadSize;
		}
		else {
			iter = mapSize;
		}

		// Перевіряємо чи не виходить промінь за межі рівня
		// та чи натраплює на стіну
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
		xh = xa; yh = ya; // Збергіаємо координати кінця променя


		////////////////////////////////////////////////
		// З двох променів обираємо коротший і        //
		// записуємо всі необхідні дані про нього     //
		// в масиви                                   //
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
