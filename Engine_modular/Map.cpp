#include "Map.h"

Map::Map()
{

}

Map::~Map()
{
    ////////////////////////////////////////////////
    // Видаляємо схему рівня                      //
    ////////////////////////////////////////////////

    //for (int i = 0; i < levelSize; i++)
    //{
    //    delete [] levelPlan[i];
    //}
    //delete[] levelPlan;
}

void Map::loadMap(int nParam, const string path)
{
    ////////////////////////////////////////////////
    //  Завантажуємо схему рівня                  //
    ////////////////////////////////////////////////

    ifstream file;
    string nStr; // Розмір рівня рядком
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
    //  Ініціалізаємо параметри мінікарти         //
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
    playerDirPointer[0].color = Color(255, 0, 0);
    playerDirPointer[1].color = Color(255, 0, 0);

    playerMarker.setRadius(minimapCellSize / 2);
    playerMarker.setFillColor(Color(255, 204, 0));

    for (int i = 0; i < levelSize; i++)
    {
        for (int j = 0; j < levelSize; j++)
        {
            quadIndex = i * levelSize * 4 + j * 4;

            x10 = x00 = j * cellSize * minimapToMapRelation;
            x11 = x01 = (j * cellSize + cellSize) * minimapToMapRelation;

            y01 = y00 = i * cellSize * minimapToMapRelation;
            y11 = y10 = (i * cellSize + cellSize) * minimapToMapRelation;

            walls[quadIndex].position = Vector2f(x00, y00);
            walls[quadIndex + 1].position = Vector2f(x01, y01);
            walls[quadIndex + 2].position = Vector2f(x11, y11);
            walls[quadIndex + 3].position = Vector2f(x10, y10);

            if (levelPlan[i][j]) quadColor = Color(255, 255, 255);
            else quadColor = Color(0, 0, 0);

            walls[quadIndex].color = quadColor;
            walls[quadIndex + 1].color = quadColor;
            walls[quadIndex + 2].color = quadColor;
            walls[quadIndex + 3].color = quadColor;
        }
    }

    ////////////////////////////////////////////////
    // Зафарбовуємо усі промені на мінікарті      // 
    ////////////////////////////////////////////////

    for (int i = 0; i < WIN_WIDTH * 2; i++)
    {
        rays[i].color = Color(255, 55, 55);
    }

    ////////////////////////////////////////////////
    // Налаштовуємо задній фон мінікарти          //
    ////////////////////////////////////////////////

    minimapBackground.setFillColor(Color(150, 150, 150));
    minimapBackground.setPosition(Vector2f(0, 0));
    minimapBackground.setSize(Vector2f(minimapSize, minimapSize));
}

void Map::draw(RenderWindow& window)
{
    window.draw(minimapBackground);
    window.draw(walls);
    window.draw(rays);
    window.draw(playerMarker);
    window.draw(playerDirPointer);
}

void Map::updateMinimap(Player& player, float** raysEndCords)
{
    ////////////////////////////////////////////////
    // Оновлюємо координати променів на мінікарті //
    ////////////////////////////////////////////////

    float xRayStart, yRayStart;
    float xRayEnd, yRayEnd;
    float xPointerEnd, yPointerEnd;
    
    xRayStart = player.getPosition().x * minimapToMapRelation;
    yRayStart = player.getPosition().y * minimapToMapRelation;

    for (int i = 0; i < WIN_WIDTH * 2; i+=2)
    {

        xRayEnd = raysEndCords[i / 2][0] * minimapToMapRelation;
        yRayEnd = raysEndCords[i / 2][1] * minimapToMapRelation;

        rays[i].position = Vector2f(xRayStart, yRayStart);
        rays[i + 1].position = Vector2f(xRayEnd, yRayEnd);
    }

    ////////////////////////////////////////////////
    // Оновлюємо вказівник напрямку руху гравця   //
    ////////////////////////////////////////////////

    xPointerEnd = (xRayStart + cellSize * cos(player.getDirection()) * minimapToMapRelation);
    yPointerEnd = (yRayStart + cellSize * sin(player.getDirection()) * minimapToMapRelation);

    playerDirPointer[0].position = Vector2f(xRayStart, yRayStart);
    playerDirPointer[1].position = Vector2f(xPointerEnd, yPointerEnd);

    ////////////////////////////////////////////////
    // Оновлюємо кординати маркерів сутностей     //
    ////////////////////////////////////////////////

    xRayStart -= playerMarker.getRadius();
    yRayStart -= playerMarker.getRadius();

    playerMarker.setPosition(Vector2f(xRayStart, yRayStart));  
}

void Map::generateMap(int mazeSize, int nParam)
{
    ////////////////////////////////////////////////
    // Генеруємо рівень                           //
    ////////////////////////////////////////////////

    levelPlan = genEmptyField(mazeSize);
    genMaze(levelPlan, mazeSize);
   
    levelSize = mazeSize;
    cellSize = 64;


    ////////////////////////////////////////////////
    //  Ініціалізаємо параметри мінікарти         //
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
    playerDirPointer[0].color = Color(255, 0, 0);
    playerDirPointer[1].color = Color(255, 0, 0);

    playerMarker.setRadius(minimapCellSize / 2);
    playerMarker.setFillColor(Color(255, 204, 0));

    for (int i = 0; i < levelSize; i++)
    {
        for (int j = 0; j < levelSize; j++)
        {
            quadIndex = i * levelSize * 4 + j * 4;

            x10 = x00 = j * cellSize * minimapToMapRelation;
            x11 = x01 = (j * cellSize + cellSize) * minimapToMapRelation;

            y01 = y00 = i * cellSize * minimapToMapRelation;
            y11 = y10 = (i * cellSize + cellSize) * minimapToMapRelation;

            walls[quadIndex].position = Vector2f(x00, y00);
            walls[quadIndex + 1].position = Vector2f(x01, y01);
            walls[quadIndex + 2].position = Vector2f(x11, y11);
            walls[quadIndex + 3].position = Vector2f(x10, y10);

            if (levelPlan[i][j]) quadColor = Color(255, 255, 255);
            else quadColor = Color(0, 0, 0);

            walls[quadIndex].color = quadColor;
            walls[quadIndex + 1].color = quadColor;
            walls[quadIndex + 2].color = quadColor;
            walls[quadIndex + 3].color = quadColor;
        }
    }

    ////////////////////////////////////////////////
    // Зафарбовуємо усі промені на мінікарті      // 
    ////////////////////////////////////////////////

    for (int i = 0; i < WIN_WIDTH * 2; i++)
    {
        rays[i].color = Color(255, 55, 55);
    }

    ////////////////////////////////////////////////
    // Налаштовуємо задній фон мінікарти          //
    ////////////////////////////////////////////////

    minimapBackground.setFillColor(Color(150, 150, 150));
    minimapBackground.setPosition(Vector2f(0, 0));
    minimapBackground.setSize(Vector2f(minimapSize, minimapSize));
}

int** Map::genEmptyField(int n)
{

    if (!(n % 2))
    {
        cout << "Maze size should be odd number!" << endl;
        return nullptr;
    }
    if (n < 3)
    {
        cout << "Maze size should be at least 3!" << endl;
        return nullptr;
    }

    int** maze = new int* [n];
    for (int i = 0; i < n; i++)
    {
        maze[i] = new int[n];
        for (int j = 0; j < n; j++)
        {
            if (i % 2 && j % 2)
                maze[i][j] = 0;
            else
                maze[i][j] = 1;
        }
    }
    return maze;
}

void Map::genMaze(int** maze, int mazeSize)
{
    stack<Vector2i> stack;
    Vector2i curCell = { 1, 1 };
    Vector2i newCell;
    int wallX, wallY;
    int cellsLeft = pow((mazeSize - 1) / 2, 2);
    int** visitedCells = genEmptyField(mazeSize);

    cellsLeft--;
    visitedCells[curCell.y][curCell.x] = 1;


    while (cellsLeft)
    {
        if (unvisitedNeighbors(visitedCells, mazeSize, curCell))
        {
            stack.push(curCell);
            newCell = findNewCell(visitedCells, mazeSize, curCell);
            wallX = (newCell.x + curCell.x) / 2;
            wallY = (newCell.y + curCell.y) / 2;
            maze[wallY][wallX] = 0;
            curCell = newCell;
            visitedCells[curCell.y][curCell.x] = 1;
            cellsLeft--;
        }
        else if (!stack.empty()) {
            curCell = stack.top();
            stack.pop();
        }
    }

    for (int i = 0; i < mazeSize; i++)
    {
        delete[]visitedCells[i];
    }

    delete[] visitedCells;

    crashWalls(maze, mazeSize);
}

void Map::crashWalls(int** maze, int mazeSize)
{
    int i, j;

    for (int n = 0; n < pow((float)mazeSize, 1.4); n++)
    {
        i = 1 + rand() % (mazeSize - 2);
        j = 1 + rand() % (mazeSize - 2);
        if ((!(maze[i - 1][j]) && !(maze[i + 1][j]) && (maze[i][j - 1]) && (maze[i][j + 1])) ||
            ((maze[i - 1][j]) && (maze[i + 1][j]) && !(maze[i][j - 1]) && !(maze[i][j + 1]))) {
            maze[i][j] = 0;
        }

    }
}

bool Map::unvisitedNeighbors(int** visited, int mazeSize, Vector2i& cell)
{
    int nX, nY;

    //---Клітинка справа---//
    nX = cell.x + 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        return 1;

    //---Клітинка зліва---//
    nX = cell.x - 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        return 1;

    //---Клітинка зверху---//
    nX = cell.x;
    nY = cell.y + 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        return 1;

    //---Клітинка знизу---//
    nX = cell.x;
    nY = cell.y - 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        return 1;

    return 0;
}

bool Map::onField(int mazeSize, int x, int y)
{
    if (x >= mazeSize - 1 || y >= mazeSize - 1 || x < 0 || y < 0)
    {
        return 0;
    }
    return 1;
}

Vector2i Map::findNewCell(int** visited, int mazeSize, Vector2i& cell)
{
    int nX, nY;
    vector<Vector2i> neighbors;
    Vector2i res;
    int index;

    //---Клітинка справа---//
    nX = cell.x + 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Vector2i{ nX, nY });

    //---Клітинка зліва---//
    nX = cell.x - 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Vector2i{ nX, nY });

    //---Клітинка зверху---//
    nX = cell.x;
    nY = cell.y + 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Vector2i{ nX, nY });

    //---Клітинка знизу---//
    nX = cell.x;
    nY = cell.y - 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Vector2i{ nX, nY });

    res = neighbors[rand() % neighbors.size()];

    return res;
}
