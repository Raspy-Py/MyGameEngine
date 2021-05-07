#include <iostream>
#include <stack>
#include <vector>
#include <time.h>
#include <Windows.h>
#include <thread>
#include <chrono>

using namespace std;

struct Cell
{
    int x;
    int y;
};

int** genEmptyField(int);
void genMaze(int**, int);
void printMatrix(int**, int, Cell cell = {10000,10000});
bool unvisitedNeighbors(int**, int, Cell& );
bool onField(int, int, int);
Cell findNewCell(int**, int, Cell&);

void printM2(int**, int);


int main()
{
    int mazeSize; // it should be odd
    int** maze;
    srand(time(NULL));


    cout << "Enter maze size(3 at least): " << endl;
    cin >> mazeSize;
    mazeSize *= 2;
    mazeSize++;

    system("cls");

    maze = genEmptyField(mazeSize);

    genMaze(maze, mazeSize);

    cout << endl;

    system("cls");
    cout << "<---Result---> "<< endl;
    printMatrix(maze, mazeSize);
    this_thread::sleep_for(chrono::milliseconds(300));

    printM2(maze, mazeSize);
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
       /* system("cls");
        cout << "Cells left " << cellsLeft << endl;
        printMatrix(maze, mazeSize, curCell);
        this_thread::sleep_for(chrono::milliseconds(mazeSize * 10));*/

        if (unvisitedNeighbors(visitedCells, mazeSize, curCell))
        {
            stack.push(curCell);
            newCell = findNewCell(visitedCells, mazeSize, curCell );
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
    system("cls");
    cout << "Cells left " << cellsLeft << endl;
    printMatrix(maze, mazeSize, curCell);
    this_thread::sleep_for(chrono::milliseconds(300));

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
            }else
            cout << char(176 + A[i][j]*2) << char(176 + A[i][j] * 2);
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
    if (x >= mazeSize-1 || y >= mazeSize-1 || x < 0 || y < 0)
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
