#include "Render.h"

RenderImage::RenderImage()
{
	////////////////////////////////////////////////
	// 	Сталі налаштування                        //
	////////////////////////////////////////////////

	image.setPrimitiveType(Lines);
	image.resize(WIN_WIDTH* TEXTURE_RES * 2);

	floor.setSize(Vector2f(WIN_WIDTH, WIN_HALF_HEIGHT));
	ceiling.setSize(Vector2f(WIN_WIDTH, WIN_HALF_HEIGHT));

	floor.setPosition(Vector2f(0, WIN_HALF_HEIGHT));
	ceiling.setPosition(Vector2f(0, 0));
}

RenderImage::~RenderImage()
{
}

void RenderImage::init(std::string pathToWallTexture, Color floorColor, Color ceilingColor)
{
	////////////////////////////////////////////////
	// 	Налаштовуємо стелю та підлогу             //    
	////////////////////////////////////////////////

	floor.setFillColor(floorColor);
	ceiling.setFillColor(ceilingColor);

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

	int R, G, B, A;

	file.seekg(bias);

	for (int i = 0; i < TEXTURE_RES * TEXTURE_RES; i++)
	{
		file.read(&buff[0], n);

		R = hexToDec(buff[2]);
		G = hexToDec(buff[1]);
		B = hexToDec(buff[0]);

		if (R + G + B == 255 * 3) A = 0;
		else A = 255;
		
		textureColorMap.push_back(Color(R,G,B,A));
	}

	file.close();

	std::reverse(textureColorMap.begin(), textureColorMap.end());

	////////////////////////////////////////////////
	// 	Розбиваємо текстуру на колонки пікселів   //
	////////////////////////////////////////////////


	//---Виділємо пам'ять під текстуру---//
	textureCols = new Color * [TEXTURE_RES];
	for (int i = 0; i < TEXTURE_RES; i++)
	{
		textureCols[i] = new Color[TEXTURE_RES];
	}

	textureColsShaded = new Color * [TEXTURE_RES];
	for (int i = 0; i < TEXTURE_RES; i++)
	{
		textureColsShaded[i] = new Color[TEXTURE_RES];
	}

	//---Ініціалізуємо колонки пікселів текстури---//
	for (int x = 0; x < TEXTURE_RES; x++)
		for (int y = 0; y < TEXTURE_RES; y++) {
			textureCols[x][y] = textureColorMap[y * TEXTURE_RES + x];
			textureColsShaded[x][y] = fadeColor(textureColorMap[y * TEXTURE_RES + x]);
		}


}

void RenderImage::updateImage(RayCasting & rc)
{
	int texCol; 
	int lineIndex;
	float wallColTop;
	float wallColSize;
	float pixelSize;

	for (int col = 0; col < WIN_WIDTH; col++)
	{
		texCol = rc.getRaysPositionsOnWalls()[col];

		wallColSize = (WALL_HEIGHT * WTP / 2) / rc.getRaysLength()[col];
		pixelSize = wallColSize / TEXTURE_RES  * 2;
		wallColTop = WIN_HALF_HEIGHT - wallColSize;

		for (int pixel = 0; pixel < TEXTURE_RES; pixel++, wallColTop+=pixelSize)
		{
			//---Зафарбовуємо стовпець стіни---//
			lineIndex = (col * TEXTURE_RES + pixel) << 1;
			
			if (rc.checkWallHorizontal()[col])
			{
				image[lineIndex].color = textureColsShaded[texCol][pixel];
				image[lineIndex + 1].color = textureColsShaded[texCol][pixel];
			}
			else {
				image[lineIndex].color = textureCols[texCol][pixel];
				image[lineIndex + 1].color = textureCols[texCol][pixel];
			}

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

void RenderImage::deleteTexture()
{
	textureColorMap.clear();
}

Color RenderImage::fadeColor(Color& original)
{
	Color faded;

	faded.r = int((float)original.r * SHADING);
	faded.g = int((float)original.g * SHADING);
	faded.b = int((float)original.b * SHADING);
	faded.a = original.a;

	return faded;
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
