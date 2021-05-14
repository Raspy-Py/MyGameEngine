#include "Render.h"

RenderImage::RenderImage()
{
	////////////////////////////////////////////////
	// 	���� ������������                        //
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

void RenderImage::init(std::string pathToWallTexture, Color& floorColor, Color& ceilingColor)
{
	////////////////////////////////////////////////
	// 	����������� ����� �� ������             //    
	////////////////////////////////////////////////

	floor.setFillColor(floorColor);
	ceiling.setFillColor(ceilingColor);

	////////////////////////////////////////////////
	// 	����������� ��������                     //
	////////////////////////////////////////////////

	const int n = 3; // �-�� ������ ������
	int bias;        // ������� ������� ������� ������� ������� �����
	char buff[n];    // �����

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
	// 	��������� �������� �� ������� ������   //
	////////////////////////////////////////////////


	//---������ ���'��� �� ��������---//
	textureCols = new Color * [TEXTURE_RES];
	for (int i = 0; i < TEXTURE_RES; i++)
	{
		textureCols[i] = new Color[TEXTURE_RES];
	}

	//---���������� ������� ������ ��������---//
	for (int x = 0; x < TEXTURE_RES; x++)
		for (int y = 0; y < TEXTURE_RES; y++)
			textureCols[x][y] = textureColorMap[y * TEXTURE_RES + x];
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
			//---����������� �������� ����---//
			lineIndex = (col * TEXTURE_RES + pixel) << 1;
			
			image[lineIndex    ].color = textureCols[textureCol][pixel];
			image[lineIndex + 1].color = textureCols[textureCol][pixel];

			//---������ ���������---//
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

unsigned RenderImage::hexToDec(char hex)
{
	unsigned dec = 0;

	for (int i = 7; i > -1; i--)
	{
		if (hex & 1 << i) { dec += 1 << i; };
	}

	return dec;
}