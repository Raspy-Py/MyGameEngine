#include "Render.h"

RenderImage::RenderImage(string pathToWallTexture)
{
	image.setPrimitiveType(Lines);
	image.resize(WIN_WIDTH * TEXTURE_RES * 2);

	floor.setPrimitiveType(Quads);
	floor.resize(4);
	floor[0].color = Color(169, 169, 169);
	floor[1].color = Color(169, 169, 169);
	floor[2].color = Color(169, 169, 169);
	floor[3].color = Color(169, 169, 169);
	floor[0].position = Vector2f(0        , WIN_HALF_HEIGHT);
	floor[1].position = Vector2f(WIN_WIDTH, WIN_HALF_HEIGHT);
	floor[2].position = Vector2f(WIN_WIDTH, WIN_HEIGHT     );
	floor[3].position = Vector2f(0        , WIN_HEIGHT     );

	ceiling.setPrimitiveType(Quads);
	ceiling.resize(4);
	ceiling[0].color = Color(112, 112, 112);
	ceiling[1].color = Color(112, 112, 112);
	ceiling[2].color = Color(112, 112, 112);
	ceiling[3].color = Color(112, 112, 112);
	ceiling[0].position = Vector2f(0        , 0              );
	ceiling[1].position = Vector2f(WIN_WIDTH, 0              );
	ceiling[2].position = Vector2f(WIN_WIDTH, WIN_HALF_HEIGHT);
	ceiling[3].position = Vector2f(0        , WIN_HALF_HEIGHT);

	////////////////////////////////////////////////
	// 	Завантажуємо текстури                     //
	////////////////////////////////////////////////

	const int n = 3; // К-ть каналів кольрів
	int bias;        // Зміщення таблиці кольорів відносно початку файлу
	char buff[n];    // Буфер

	ifstream file(pathToWallTexture, ios::binary | ios::in);

	file.seekg(0x0A);
	file.read(&buff[0], 3);
	bias = hexToDec(buff[0]);

	file.seekg(bias);

	for (int i = 0; i < TEXTURE_RES * TEXTURE_RES; i++)
	{
		file.read(&buff[0], n);

		textureColorMap.push_back(Color(
			hexToDec(buff[2]),
			hexToDec(buff[1]),
			hexToDec(buff[0])
		));
	}

	file.close();

	////////////////////////////////////////////////
	// 	Розбиваємо текстуру на колонки пікселів   //
	////////////////////////////////////////////////


	//---Виділємо пам'ять під текстуру---//
	textureCols = new Color * [TEXTURE_RES];
	for (int i = 0; i < TEXTURE_RES; i++) 
	{ 
		textureCols[i] = new Color[TEXTURE_RES]; 
	}

	//---Ініціалізуємо колонки пікселів текстури---//
	for (int x = 0; x < TEXTURE_RES; x++)
		for (int y = 0; y < TEXTURE_RES; y++)
			textureCols[x][y] = textureColorMap[y * TEXTURE_RES + x];
}

RenderImage::~RenderImage()
{
}

void RenderImage::init()
{
	
}

void RenderImage::updateImage(RayCasting & rc)
{
	int textureCol; 
	int lineIndex;
	float wallColTop;
	float wallColSize;
	float pixelSize;

	for (int col = 0; col < WIN_WIDTH; col++)
	{
		textureCol = rc.raysPositionsOnWalls[col];

		wallColSize = (WALL_HEIGHT * WTP / 2) / rc.raysLength[col];
		pixelSize = wallColSize / TEXTURE_RES  * 2;
		wallColTop = WIN_HALF_HEIGHT - wallColSize;

		for (int pixel = 0; pixel < TEXTURE_RES; pixel++, wallColTop+=pixelSize)
		{
			//---Зафарбовуємо стовпець стіни---//
			lineIndex = (col * TEXTURE_RES + pixel) << 1;
			
			image[lineIndex    ].color = textureCols[textureCol][pixel];
			image[lineIndex + 1].color = textureCols[textureCol][pixel];

			//---Задаємо кординати---//
			image[lineIndex    ].position = Vector2f(col, wallColTop);
			image[lineIndex + 1].position = Vector2f(col, wallColTop+ pixelSize);
		}
	}
}

void RenderImage::draw(RenderWindow& window)
{
	window.draw(floor);
	window.draw(ceiling);
	window.draw(image);
}

unsigned RenderImage::hexToDec(char hex)
{
	unsigned dec = 0;

	for (int i = 7; i > -1; i--)
	{
		if (hex & 1 << i) { dec += 1 << i; };
	}

	return dec;
}
