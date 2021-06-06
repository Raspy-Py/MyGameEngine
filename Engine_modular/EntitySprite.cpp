#include "EntitySprite.h"

EntitySprite::EntitySprite()
{
	spriteLeftFirstCol = 0;
	shouldBeDisplayed = false;
}

EntitySprite::~EntitySprite()
{
}

void EntitySprite::draw(RenderWindow& window, RayCasting& rc)
{
	if (shouldBeDisplayed)
	for (int i = 0; i < actualSprite.size(); i++)
	{
		if (distToPlayer < rc.getRaysLength()[i + leftSideColOnDisplay])
		{
			window.draw(actualSprite[i]);
		}
	}
}

void EntitySprite::loadSprite(std::string path, int spriteSize)
{
	////////////////////////////////////////////////
	// 	Завантажуємо текстури                     //
	////////////////////////////////////////////////

	const int n = 3; // К-ть каналів кольорів
	int bias;        // Зміщення таблиці кольорів відносно початку файлу
	char buff[n];    // Буфер

	std::vector<Color> textureColorMap;
	std::ifstream file(path, std::ios::binary | std::ios::in);

	file.seekg(0x0A);
	file.read(&buff[0], 3);
	bias = hexToDec(buff[0]);

	int R, G, B, A;

	file.seekg(bias);

	for (int i = 0; i < spriteSize * spriteSize; i++)
	{
		file.read(&buff[0], n);

		R = hexToDec(buff[2]);
		G = hexToDec(buff[1]);
		B = hexToDec(buff[0]);

		if (R + G + B == 255 * 3) A = 0;
		else A = 255;

		textureColorMap.push_back(Color(R, G, B, A));
	}

	file.close();

	std::reverse(textureColorMap.begin(), textureColorMap.end());

	spriteTex = new Color*[MONSTER_SPRITE_RES];
	for (int i = 0; i < MONSTER_SPRITE_RES; i++)
		spriteTex[i] = new Color[MONSTER_SPRITE_RES];

	//---Ініціалізуємо колонки пікселів зображення---//
	for (int x = 0; x < spriteSize; x++)
		for (int y = 0; y < spriteSize; y++) {
			spriteTex[x][y] = textureColorMap[y * spriteSize + x];
		}
}

unsigned EntitySprite::hexToDec(char hex)
{
	unsigned dec = 0;

	for (int i = 7; i > -1; i--)
	{
		if (hex & 1 << i) { dec += 1 << i; };
	}

	return dec;
}

void EntitySprite::calculateSprite(Player& player, Vector2f entityPos)
{
	Vector2f dirToEntity = {
		 entityPos - player.getPosition()
	};

	shouldBeDisplayed = isInFieldOfView(player.getDirection(), dirToEntity);

	float distToEntity;
	float distToEntityProj;
	float enemyCentreAngle;
	float enemySize = MONSTER_SPRITE_RES;
	float enemyProjSize;
	float texPixelHeight;
	float spriteTopPos;
	int currentCol;
	int rightSideColOnDisplay;
	int centreColOnDisplay;
	int numberOfCols;
	int start;
	int stop;



	if (shouldBeDisplayed) {
		distToEntity = sqrt(
			dirToEntity.x * dirToEntity.x +
			dirToEntity.y * dirToEntity.y
		);
		distToPlayer = distToEntity;
		
		if (distToEntity < 15) {
			shouldBeDisplayed = false;
			return;
		}

		enemyCentreAngle = vectorToAngle(dirToEntity);

		float a = player.getDirection() - enemyCentreAngle;
		if (a < -ANGLE_180)
			a += ANGLE_360;
		if (a > ANGLE_180)
			a -= ANGLE_360;
		centreColOnDisplay = WIN_HALF_WIDTH- (a) / ABR;

		a = enemyCentreAngle - player.getDirection();
		if (a < -ANGLE_180)
			a += ANGLE_360;
		if (a > ANGLE_180)
			a -= ANGLE_360;
		distToEntityProj = abs(distToEntity * cos(a));

		enemyProjSize = float(MONSTER_SPRITE_RES) * WTP / distToEntityProj/2;

		
		leftSideColOnDisplay = centreColOnDisplay - enemyProjSize;
		rightSideColOnDisplay = centreColOnDisplay + enemyProjSize;
		spriteLeftFirstCol = leftSideColOnDisplay;

		numberOfCols = rightSideColOnDisplay - leftSideColOnDisplay;
	

		texPixelHeight = enemyProjSize / MONSTER_SPRITE_RES * 2;

		spriteTopPos = WIN_HALF_HEIGHT - enemyProjSize;

		actualSprite.clear();


		for (int i = 0; i < numberOfCols; i++)
		{
			currentCol = float(i) / numberOfCols * MONSTER_SPRITE_RES;
			
			actualSprite.push_back(
				VertexArray(Lines, MONSTER_SPRITE_RES * 2)
			);

			for (int j = 0; j < MONSTER_SPRITE_RES; j++)
			{
				actualSprite[i][j * 2 + 0].color = spriteTex[currentCol][j];
				actualSprite[i][j * 2 + 1].color = spriteTex[currentCol][j];

				actualSprite[i][j * 2 + 0].position = Vector2f(leftSideColOnDisplay + i, spriteTopPos + j * texPixelHeight);
				actualSprite[i][j * 2 + 1].position = Vector2f(leftSideColOnDisplay + i, spriteTopPos + (j + 1) * texPixelHeight);

			}
		}

	}

}

double EntitySprite::vectorToAngle(Vector2f vect)
{
	double cosA;
	double len = 0.000000001;
	double angle;

	len += sqrt(vect.x * vect.x + vect.y * vect.y);

	cosA = vect.x / len;


	angle = acos(cosA);

	if (vect.y < 0)
	{
		angle = ANGLE_360 - angle;
	}

	return angle;
}

bool EntitySprite::isInFieldOfView(float dir, Vector2f dirToEntity)
{
	float a = vectorToAngle(dirToEntity);

	a -= dir;

	if (a < -ANGLE_180)
		a += ANGLE_360;
	if (a > ANGLE_180)
		a -= ANGLE_360;

	bool res = (a <= ANGLE_90/3 && a >= -ANGLE_90/3);
	return res;
}
 