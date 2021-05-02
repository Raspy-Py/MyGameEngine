#include "Map.h"

Map::Map(int nParam, const string path)
{
    ////////////////////////////////////////////////
    //  ����������� ����� ����                  //
    ////////////////////////////////////////////////

    ifstream file;
    string nStr; // ����� ���� ������
    string cellStr;
    int cell;
 

    file.open(path);

    if (!file.is_open()) {
        cout << "Failed to load game map! File opening error.\n";
    }

    getline(file, nStr, '\n');
    levelSize = stoi(nStr);

    getline(file, nStr, '\n');
    cellSize = stoi(nStr);

    levelPlan = new int* [levelSize];

    for (int i = 0; i < levelSize; i++)
    {
        levelPlan[i] = new int[levelSize];

        for (int j = 0; j < levelSize - 1; j++)
        {
            getline(file, cellStr, ',');
            cell = stoi(cellStr);
            levelPlan[i][j] = cell;
        }
        getline(file, cellStr, '\n');
        cell = stoi(cellStr);
        levelPlan[i][levelSize - 1] = cell;
    }
    file.close();


    ////////////////////////////////////////////////
    //  ���������� ��������� �������         //
    ////////////////////////////////////////////////

    int x00, x01, x10, x11;
    int y00, y01, y10, y11;
    int quadIndex;
    Color quadColor;

    minimapToMapRelation = float(nParam) / (levelSize * cellSize);

    minimapSize = nParam;
    minimapCellSize = minimapSize / levelSize;

    walls.resize(levelSize * levelSize * 4);
    walls.setPrimitiveType(Quads);

    rays.resize(WIN_WIDTH * 2);
    rays.setPrimitiveType(Lines);

    playerDirPointer.resize(2);
    playerDirPointer.setPrimitiveType(Lines);

    playerMarker.setRadius(minimapCellSize / 2);
    playerMarker.setFillColor(Color(0, 230, 0));

    for (int i = 0; i < levelSize; i++)
    {
        for (int j = 0; j < levelSize; j++)
        {
            quadIndex = i * levelSize * 4 + j * 4;

            x10 = x00 = j * cellSize * minimapToMapRelation;
            x11 = x01 = (j * cellSize + cellSize) * minimapToMapRelation;

            y01 = y00 = i * cellSize * minimapToMapRelation;
            y11 = y10 = (i * cellSize + cellSize) * minimapToMapRelation;

            walls[quadIndex    ].position = Vector2f(x00, y00);
            walls[quadIndex + 1].position = Vector2f(x01 - 1 , y01);
            walls[quadIndex + 2].position = Vector2f(x11 - 1, y11 - 1);
            walls[quadIndex + 3].position = Vector2f(x10, y10 - 1);

            if (levelPlan[i][j]) quadColor = Color(255, 255, 255);                
            else quadColor = Color(0, 0, 0);
                
            walls[quadIndex    ].color = quadColor;
            walls[quadIndex + 1].color = quadColor;
            walls[quadIndex + 2].color = quadColor;
            walls[quadIndex + 3].color = quadColor;
        }
    }

    ////////////////////////////////////////////////
    // ����������� �� ������ �� ������      // 
    ////////////////////////////////////////////////

    for (int i = 0; i < WIN_WIDTH * 2; i++)
    {
        rays[i].color = Color(255, 55, 55);
    }

    ////////////////////////////////////////////////
    // ����������� ����� ��� �������          //
    ////////////////////////////////////////////////

    minimapBackground.setFillColor(Color(150, 150, 150));
    minimapBackground.setPosition(Vector2f(0, 0));
    minimapBackground.setSize(Vector2f(minimapSize, minimapSize));


    /////////////////////////////////////////////////
    //// ���������� ���!!!!!!!!!
    /////////////////////////////////////////////////

    //ifstream file1("../data/textures/photo.bmp", ios::binary | ios::in);
    //char buff[3]; // ����� �������
    //int start = 0x36; // ������� 

    //int clr[3]; // �������: b g r
    //int temp;

    //file1.seekg(start);

    //for (int i = 0; i < 64 * 64; i++)
    //{
    //    file1.read(&buff[0], 3);
    //    clr[0] = 0;
    //    clr[1] = 0;
    //    clr[2] = 0;

    //    for (int j = 0; j < 3; j++)
    //    {
    //        temp = 0;
    //        for (int k = 7; k > -1; k--)
    //        {
    //            if (buff[j] & (1 << k))
    //            {
    //                temp += (1 << k);
    //            }
    //        }
    //        clr[j] = temp;
    //    }

    //    walls[(i * 4) ].color = Color(clr[2], clr[1], clr[0]);
    //    walls[(i * 4 + 1)].color = Color(clr[2], clr[1], clr[0]);
    //    walls[(i * 4 + 2) ].color = Color(clr[2], clr[1], clr[0]);
    //    walls[(i * 4 + 3) ].color = Color(clr[2], clr[1],clr[0]);
    //}
}

Map::~Map()
{
    ////////////////////////////////////////////////
    // ��������� ����� ����                      //
    ////////////////////////////////////////////////

    //for (int i = 0; i < levelSize; i++)
    //{
    //    delete [] levelPlan[i];
    //}
    //delete[] levelPlan;
}

void Map::draw(RenderWindow& window)
{
    window.draw(minimapBackground);
    window.draw(walls);
    window.draw(rays);
    window.draw(playerMarker);
    window.draw(playerDirPointer);
}

void Map::updateMinimap(Player player, float** raysEndCords)
{
    ////////////////////////////////////////////////
    // ��������� ���������� ������� �� ������ //
    ////////////////////////////////////////////////

    float xRayStart, yRayStart;
    float xRayEnd, yRayEnd;
    float xPointerEnd, yPointerEnd;
    

    for (int i = 0; i < WIN_WIDTH * 2; i+=2)
    {
        xRayStart = player.position.x * minimapToMapRelation;
        yRayStart = player.position.y * minimapToMapRelation;

        xRayEnd = raysEndCords[i / 2][0] * minimapToMapRelation;
        yRayEnd = raysEndCords[i / 2][1] * minimapToMapRelation;

        rays[i].position = Vector2f(xRayStart, yRayStart);
        rays[i + 1].position = Vector2f(xRayEnd, yRayEnd);
    }

    ////////////////////////////////////////////////
    // ��������� �������� �������� ���� ������   //
    ////////////////////////////////////////////////

    xPointerEnd = (xRayStart + 64 * cos(player.direction) * minimapToMapRelation);
    yPointerEnd = (yRayStart + 64 * sin(player.direction) * minimapToMapRelation);

    playerDirPointer[0].position = Vector2f(xRayStart, yRayStart);
    playerDirPointer[1].position = Vector2f(xPointerEnd, yPointerEnd);

    ////////////////////////////////////////////////
    // ��������� ��������� ������� ���������     //
    ////////////////////////////////////////////////

    xRayStart -= playerMarker.getRadius();
    yRayStart -= playerMarker.getRadius();

    playerMarker.setPosition(Vector2f(xRayStart, yRayStart));

   
}