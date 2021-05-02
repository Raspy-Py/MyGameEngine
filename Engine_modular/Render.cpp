#include "Render.h"

RenderImage::RenderImage(string pathToWallTexture)
{
	image.setPrimitiveType(Quads);
	image.resize(4096);

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

unsigned RenderImage::hexToDec(char hex)
{
	unsigned dec = 0;

	for (int i = 7; i > -1; i--)
	{
		if (hex & 1 << i) { dec += 1 << i; };
	}

	return dec;
}
