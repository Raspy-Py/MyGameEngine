#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <time.h>

using namespace std;
using namespace sf;

const int WIN_SIZE = 720;

const string DIR = "../data/levels/";

int** getConfig(int&, int&, VertexArray&);
void drawWall(int**, VertexArray&, int&, RenderWindow&);
void saveMap(int**, int&, int&, string&);

enum class NodeType {
    UNCHECKED,
    OPEN,
    CLOSED,
    WALL
};

struct Node {
    int g;
    int h;
    int f;
    Vector2i gridPos;
    Node* parent;
    NodeType type;
};

int** genEmptyField(int);
void updateMapImage(int**, VertexArray&, int&);
void crashWalls(int**, int);
void genMaze(int**, int);
void printMatrix(int**, int, Vector2i cell = { 10000,10000 });
bool unvisitedNeighbors(int**, int, Vector2i&);
bool onField(int, int, int);
Vector2i findNewCell(int**, int, Vector2i&);

vector<Vector2i> aStarPathFind(int**, int, const Vector2i&, const Vector2i&);
Node* chooseCurrentNode(vector<Node*>&);
int distance(Vector2i, Vector2i);
void removeFromOpenAndClose(Node*, vector<Node*>&);
void displayPath(vector<Vector2i>&, VertexArray&, int);


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

    //---Path finding data---//
    Vector2i start = {-1,-1};
    Vector2i target;
    vector<Vector2i> path;

    //---Primary settings--//
    cells.setPrimitiveType(Quads);

    background.setSize(Vector2f(WIN_SIZE, WIN_SIZE));
    background.setPosition(Vector2f(0, 0));
    background.setFillColor(Color(150, 150, 150));

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

        if (Keyboard::isKeyPressed(Keyboard::G))
        {
            elapsed = time.getElapsedTime();
            if (elapsed.asMilliseconds() > 200)
            {
                if (mapSize % 2)
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
        else if (Keyboard::isKeyPressed(Keyboard::C)) {
            elapsed = time.getElapsedTime();
            if (elapsed.asMilliseconds() > 200)
            {
                crashWalls(map, mapSize);
                updateMapImage(map, cells, mapSize);

                time.restart();
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            float cellSize = (float)WIN_SIZE / mapSize;

            Vector2i mPos = Mouse::getPosition(window);

            if (Mouse::isButtonPressed(Mouse::Left))
            {

                if (mPos.x < WIN_SIZE && mPos.x >= 0 && mPos.y < WIN_SIZE && mPos.y >=0)
                {

                    int i = mPos.y / cellSize;
                    int j = mPos.x / cellSize;

                    target = { i,j };

                    Color quadColor = Color(255, 80, 80);

                    cells[(i * mapSize + j) * 4 + 0].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 1].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 2].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 3].color = quadColor;

                    // Знаходимо шлях
                    elapsed = time.getElapsedTime();
                    if (elapsed.asMilliseconds() > 200 &&
                        start.x != -1)
                    {
                        path = aStarPathFind(map, mapSize, start, target);
                        //cout << path.size() << endl;

                        displayPath(path, cells, mapSize);

                        time.restart();
                    }

                }
            }
            else if (Mouse::isButtonPressed(Mouse::Right))
            {
                updateMapImage(map, cells, mapSize);

                if (mPos.x < WIN_SIZE && mPos.x >= 0 && mPos.y < WIN_SIZE && mPos.y >=0)
                {
                    int i = mPos.y / cellSize;
                    int j = mPos.x / cellSize;

                    start = { i, j };

                    Color quadColor = Color(80, 80, 255);

                    cells[(i * mapSize + j) * 4 + 0].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 1].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 2].color = quadColor;
                    cells[(i * mapSize + j) * 4 + 3].color = quadColor;
                }
            }
        }
        else
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

    mCells.resize(mSize * mSize * 4);

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

            if (i == 0 || j == 0 || i == mSize - 1 || j == mSize - 1)
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
    }
    else if (Mouse::isButtonPressed(Mouse::Right))
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

    file.open(DIR + fileName);

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
                quadColor = Color(255, 255, 255);
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

    //crashWalls(maze, mazeSize);
}

void crashWalls(int** maze, int mazeSize)
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

void printMatrix(int** A, int n, Vector2i cell)
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

bool unvisitedNeighbors(int** visited, int mazeSize, Vector2i& cell)
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

Vector2i findNewCell(int** visited, int mazeSize, Vector2i& cell)
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

vector<Vector2i> aStarPathFind(int** maze, int mazeSize, const Vector2i& start, const Vector2i& target)
{
    //---Для замірювання часу---//
    Clock time;
    Time elapsed;
    time.restart(); 

    vector<Vector2i> path;
    vector<Node*> openNodes;
    vector<Vector2i> deltas = { Vector2i(-1,0),Vector2i(1,0),Vector2i(0,-1),Vector2i(0,1) };
    Node* current;
    Node* neighbour;

    //---Створюємо та ініціалізуємо граф---//

    Node** graph = new Node * [mazeSize];
    for (int i = 0; i < mazeSize; i++)
    {
        graph[i] = new Node[mazeSize];

        for (int j = 0; j < mazeSize; j++)
        {
            graph[i][j].gridPos.x = j;
            graph[i][j].gridPos.y = i;
            
            if (maze[j][i])
            {
                graph[i][j].type = NodeType::WALL;
            }
            else {
                graph[i][j].type = NodeType::UNCHECKED;
            }
        }
    }

    //---Алгоритм---//
    int i;
    int j;

    graph[start.y][start.x].type = NodeType::OPEN;
    graph[start.y][start.x].g = 0;
    graph[start.y][start.x].h = distance(graph[start.y][start.x].gridPos, target);
    graph[start.y][start.x].f = graph[start.y][start.x].h;
    graph[start.y][start.x].parent = nullptr;
    openNodes.push_back(&graph[start.y][start.x]);

    while (true)
    {

        //---Крок 1---//
        current = chooseCurrentNode(openNodes);
        removeFromOpenAndClose(current, openNodes);


        //---Крок 2---//
        if (current->gridPos == target)
            break;

        //---Крок 3---//
        i = current->gridPos.y;
        j = current->gridPos.x;

        for (auto d = deltas.begin(); d != deltas.end(); ++d)
        {
            neighbour = &graph[i + d->y][j + d->x];

            if ((!d->x && !d->y) ||
                neighbour->type == NodeType::WALL ||
                neighbour->type == NodeType::CLOSED
                ) continue;

            if (neighbour->type != NodeType::OPEN ||
                (current->g + distance(neighbour->gridPos, target) + 1)
                < neighbour->f)
            {
                neighbour->g = current->g + 1;
                neighbour->h = distance(neighbour->gridPos, target);
                neighbour->f = neighbour->g + neighbour->h;
                neighbour->parent = current;
                neighbour->type = NodeType::OPEN;
                openNodes.push_back(neighbour);
            }
        }
    }

    //---Формуємо шлях---//
    
    Node* nextNode = &graph[target.y][target.x];
    path.push_back(nextNode->gridPos);

    while (nextNode->parent)
    {
        nextNode = nextNode->parent;
        path.push_back(nextNode->gridPos);
    }


    //---Чистимо пам'ять---//
    for (int i = 0; i < mazeSize; i++)
    {
        delete[] graph[i];
    }
    delete[] graph;

    elapsed = time.getElapsedTime();

    cout << "Path found in: " << elapsed.asMilliseconds() << "ms" << endl;

    return path;
}

Node* chooseCurrentNode(vector<Node*>& openNodes) {
    Node* chosenNode = openNodes[0];

    for (int i = 1; i < openNodes.size(); i++)
    {
        if (chosenNode->f > openNodes[i]->f)
        {
            chosenNode = openNodes[i];
        }
    }

    return chosenNode;
}

int distance(Vector2i a, Vector2i b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void removeFromOpenAndClose(Node* node, vector<Node*>& openNodes) {
    node->type = NodeType::CLOSED;

    for (auto iterNode = openNodes.begin(); iterNode != openNodes.end(); ++iterNode)
    {
        if ((*iterNode)->gridPos.x == node->gridPos.x && 
            (*iterNode)->gridPos.y == node->gridPos.y)
        {
            openNodes.erase(iterNode);
            break;
        }
    }
}

void displayPath(vector<Vector2i>& path, VertexArray& cells, int mapSize)
{
    for (auto d = path.begin(); d != path.end() - 1; ++d)
    {
        Color quadColor = Color(80, 255, 80);

        cells[(d->x * mapSize + d->y) * 4 + 0].color = quadColor;
        cells[(d->x * mapSize + d->y) * 4 + 1].color = quadColor;
        cells[(d->x * mapSize + d->y) * 4 + 2].color = quadColor;
        cells[(d->x * mapSize + d->y) * 4 + 3].color = quadColor;
    }
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
