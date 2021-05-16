#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <chrono>

using namespace std;
using namespace sf;

const int WIN_SIZE = 640;

const string DIR = "../data/levels/";

int** getConfig(int&, int&, VertexArray&);
void drawWall(int**, VertexArray&, int&, RenderWindow&);
void saveMap(int**, int&, int&, string&);

struct Cell
{
    int x;
    int y;
};

int** genEmptyField(int);
void updateMapImage(int**, VertexArray&, int&);
void genMaze(int**, int);
void printMatrix(int**, int, Cell cell = { 10000,10000 });
bool unvisitedNeighbors(int**, int, Cell&);
bool onField(int, int, int);
Cell findNewCell(int**, int, Cell&);


int main() {
    Clock time;
    Time elapsed;
    time.restart();

	int mapSize;
	int cellSize;
	int** map;
	VertexArray cells;
	RectangleShape background;
	string mapName;

	//---Primary settings--//
	cells.setPrimitiveType(Quads);

	background.setSize(Vector2f(WIN_SIZE, WIN_SIZE));
	background.setPosition(Vector2f(0, 0));
	background.setFillColor(Color(150,150,150));

	cout << "Enter file name: " << endl;
	getline(cin, mapName);

	map = getConfig(cellSize, mapSize, cells);

	RenderWindow window(VideoMode(WIN_SIZE, WIN_SIZE), "Map Editor", Style::Titlebar | Style::Close);

	while (window.isOpen())
	{
		// Обробка подій
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::LControl) && Keyboard::isKeyPressed(Keyboard::S))
		{
            elapsed = time.getElapsedTime();
            if (elapsed.asMilliseconds() > 200)
            {
                saveMap(map, mapSize, cellSize, mapName);
                cout << "Map saved!" << endl;

                time.restart();
            }
		}
        else if (Keyboard::isKeyPressed(Keyboard::G))
        {
            elapsed = time.getElapsedTime();
            if (elapsed.asMilliseconds() > 200)
            {
                if (mapSize%2)
                {
                    for (int i = 0; i < mapSize; i++)
                    {
                        delete[] map[i];
                    }
                    delete[]map;

                    map = genEmptyField(mapSize);
                    genMaze(map, mapSize);
                    updateMapImage(map, cells, mapSize);
                }
                else
                    cout << "Maze size should be odd!" << endl;

                time.restart();
            }
        }

		drawWall(map, cells, mapSize, window);

		window.clear();
		window.draw(background);
		window.draw(cells);
		window.display();
	}

	
	return 0;
}


int** getConfig(int& cSize, int& mSize, VertexArray& mCells)
{
	int** mPlan;
	cout << "Enter size of the map: "; cin >> mSize;
	cout << "Enter size of the cell: "; cin >> cSize;

	mCells.resize(mSize*mSize*4);

	float quadSize = (float)WIN_SIZE / mSize;
	float quadX, quadY;
	Color quadColor;

	mPlan = new int* [mSize];

	for (int i = 0; i < mSize; i++)
	{
		mPlan[i] = new int[mSize];

		for (int j = 0; j < mSize; j++)
		{
			quadX = j * quadSize;
			quadY = i * quadSize;

			mCells[(i * mSize + j) * 4 + 0].position = Vector2f(quadX, quadY);
			mCells[(i * mSize + j) * 4 + 1].position = Vector2f(quadX + quadSize - 1, quadY);
			mCells[(i * mSize + j) * 4 + 2].position = Vector2f(quadX + quadSize - 1, quadY + quadSize - 1);
			mCells[(i * mSize + j) * 4 + 3].position = Vector2f(quadX, quadY + quadSize - 1);

			if (i == 0 || j == 0 || i == mSize-1 || j == mSize-1)
			{
				mPlan[i][j] = 1;
				quadColor = Color(255, 255, 255);
			}
			else {
				mPlan[i][j] = 0;
				quadColor = Color(0, 0, 0);
			}

			mCells[(i * mSize + j) * 4 + 0].color = quadColor;
			mCells[(i * mSize + j) * 4 + 1].color = quadColor;
			mCells[(i * mSize + j) * 4 + 2].color = quadColor;
			mCells[(i * mSize + j) * 4 + 3].color = quadColor;
		}
	}

	return mPlan;
}

void drawWall(int** map, VertexArray& cells, int& mapSize, RenderWindow& window)
{
	float cellSize = (float)WIN_SIZE / mapSize;

	Vector2i mPos = Mouse::getPosition(window);
	
	if (Mouse::isButtonPressed(Mouse::Left))
	{

		if (mPos.x < WIN_SIZE && mPos.x >= 0 && mPos.y < WIN_SIZE && mPos.y)
		{
			int i = mPos.y / cellSize;
			int j = mPos.x / cellSize;

			map[i][j] = 1;

			Color quadColor = Color(255, 255, 255);

			cells[(i * mapSize + j) * 4 + 0].color = quadColor;
			cells[(i * mapSize + j) * 4 + 1].color = quadColor;
			cells[(i * mapSize + j) * 4 + 2].color = quadColor;
			cells[(i * mapSize + j) * 4 + 3].color = quadColor;
		}
	}else if (Mouse::isButtonPressed(Mouse::Right))
	{
		if (mPos.x < WIN_SIZE && mPos.x >= 0 && mPos.y < WIN_SIZE && mPos.y)
		{
			int i = mPos.y / cellSize;
			int j = mPos.x / cellSize;

			map[i][j] = 0;

			Color quadColor = Color(0, 0, 0);

			cells[(i * mapSize + j) * 4 + 0].color = quadColor;
			cells[(i * mapSize + j) * 4 + 1].color = quadColor;
			cells[(i * mapSize + j) * 4 + 2].color = quadColor;
			cells[(i * mapSize + j) * 4 + 3].color = quadColor;
		}
	}
}

void saveMap(int** map, int& mapSize, int& cellSize, string& fileName)
{
	ofstream file;

	//---Check file name--///

	if (fileName.substr(fileName.size() - 4, 4) != ".csv")
		fileName += ".csv";

	file.open(DIR+fileName);

	file << mapSize << endl;
	file << cellSize << endl;

	for (int i = 0; i < mapSize; i++)
	{
		for (int j = 0; j < mapSize; j++)
		{
			file << map[i][j];
			if (i != mapSize - 1 || j != mapSize - 1)
				file << (j != mapSize - 1 ? ',' : '\n');
		}
	}

	file.close();
}

int** genEmptyField(int n)
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

void updateMapImage(int** map, VertexArray& cells, int& mapSize)
{
    Color quadColor;

    for (int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < mapSize; j++)
        {
            if (map[i][j])
                quadColor = Color(255,255,255);
            else
                quadColor = Color(0, 0, 0);

            
            cells[(i * mapSize + j) * 4 + 0].color = quadColor;
            cells[(i * mapSize + j) * 4 + 1].color = quadColor;
            cells[(i * mapSize + j) * 4 + 2].color = quadColor;
            cells[(i * mapSize + j) * 4 + 3].color = quadColor;
        }
    }
}

void genMaze(int** maze, int mazeSize)
{
    stack<Cell> stack;
    Cell curCell = { 1, 1 };
    Cell newCell;
    int wallX, wallY;
    int cellsLeft = pow((mazeSize - 1) / 2, 2);
    int** visitedCells = genEmptyField(mazeSize);

    cellsLeft--;
    visitedCells[curCell.y][curCell.x] = 1;


    while (cellsLeft)
    {
      /*  system("cls");
        cout << "Cells left " << cellsLeft << endl;
        printMatrix(maze, mazeSize, curCell);
        this_thread::sleep_for(chrono::milliseconds(mazeSize * 10));*/

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
   /* system("cls");
    cout << "Cells left " << cellsLeft << endl;
    printMatrix(maze, mazeSize, curCell);
    this_thread::sleep_for(chrono::milliseconds(300));*/

    for (int i = 0; i < mazeSize; i++)
    {
        delete[]visitedCells[i];
    }

    delete[] visitedCells;

    /*while (!stack.empty()) {
        stack.pop();
    }*/

}

void printMatrix(int** A, int n, Cell cell)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == cell.y && j == cell.x)
            {
                cout << "##";
            }
            else
                cout << char(176 + A[i][j] * 2) << char(176 + A[i][j] * 2);
        }
        cout << endl;
    }
}

bool unvisitedNeighbors(int** visited, int mazeSize, Cell& cell)
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

bool onField(int mazeSize, int x, int y)
{
    if (x >= mazeSize - 1 || y >= mazeSize - 1 || x < 0 || y < 0)
    {
        return 0;
    }
    return 1;
}

Cell findNewCell(int** visited, int mazeSize, Cell& cell)
{
    int nX, nY;
    vector<Cell> neighbors;
    Cell res;
    int index;

    //---Клітинка справа---//
    nX = cell.x + 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Cell{ nX, nY });

    //---Клітинка зліва---//
    nX = cell.x - 2;
    nY = cell.y;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Cell{ nX, nY });

    //---Клітинка зверху---//
    nX = cell.x;
    nY = cell.y + 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Cell{ nX, nY });

    //---Клітинка знизу---//
    nX = cell.x;
    nY = cell.y - 2;
    if (onField(mazeSize, nX, nY) && !visited[nY][nX])
        neighbors.push_back(Cell{ nX, nY });

    res = neighbors[rand() % neighbors.size()];

    return res;
}

void printM2(int** A, int n)
{
    cout << n << "\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << A[i][j] << (j == n - 1 ? '\n' : ',');
        }
    }

}
