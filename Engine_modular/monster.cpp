#include "Monster.h"
#include <time.h>

int Monster::monsterLeft = 0;

Monster::Monster()
{
    monsterLeft = NUMBER_OF_MONSTERS;
}

Monster::~Monster()
{
    monsterLeft = 0;
}

void Monster::makeStep(FPS& fps, Map& map, Player& player)
{
    if (pathToPlayer.size() && 
        !distance(playerLastPosition , Vector2i(
            (int)player.getPosition().x / map.getCellSize(), 
            (int)player.getPosition().y / map.getCellSize()))) {

        if (isPointReached(map)) {
            pathToPlayer.erase(pathToPlayer.begin());
        }
        else {
            float dx, dy;
            Vector2f node = {
                float(pathToPlayer[0].x * map.getCellSize() + map.getCellSize() / 2),
                float(pathToPlayer[0].y * map.getCellSize() + map.getCellSize() / 2)
            };

            dx = (node.x - position.x) / sqrt(sqrDistance(node, position)) * speed * fps.getFPS();
            dy = (node.y - position.y) / sqrt(sqrDistance(node, position)) * speed * fps.getFPS();

            position.x += dx;
            position.y += dy;
        }
    }
    else {
        Vector2i start;
        Vector2i target;

        start = {
            (int)player.getPosition().x / map.getCellSize(),
            (int)player.getPosition().y / map.getCellSize()
        };
        target = {
            (int)position.x / map.getCellSize(),
            (int)position.y / map.getCellSize()
        };

        playerLastPosition = start;
        pathToPlayer = aStarPathFind(map.getLevelPlan(), map.getLevelSize(), start, target);

        makeStep(fps, map, player);
    }
}

void Monster::checkStatus(Player& player, Map& map, EntitySprite& sprite) {
    if (!monsterLeft)
    {
        return;
    }
    
    if (health > 0) {

        if (sqrt(sqrDistance(position, player.getPosition())) <
            MONSTER_ATACK_DISTANCE) {
            player.setHealth(player.getHealth() - damage);
         }
    }
    else {
        monsterLeft--;
        respawn(map, player.getPosition(), sprite);
    }
}

float Monster::sqrDistance(const Vector2f& a,const Vector2f& b)
{
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

std::vector<Vector2i> Monster::aStarPathFind(int** maze, int mazeSize, const Vector2i& start, const Vector2i& target)
{
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

            if (maze[i][j])
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
    if(path.size() > 1)
    path.erase(path.begin());
    //---Чистимо пам'ять---//
    for (int i = 0; i < mazeSize; i++)
    {
        delete[] graph[i];
    }
    delete[] graph;

    return path;
}

Node* Monster::chooseCurrentNode(std::vector<Node*>& openNodes)
{
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

int Monster::distance(Vector2i a, Vector2i b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void Monster::removeFromOpenAndClose(Node* node, std::vector<Node*>& openNodes)
{
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

void Monster::respawn(Map& map, const Vector2f& plPos, EntitySprite& sprite)
{
    float minDistToPlayer = float(map.getCellSize() / 3) * map.getLevelSize();
    int i, j;
    Vector2f newPos;
    pathToPlayer.clear();

    if (monsterLeft > NUMBER_OF_MONSTERS / 2)
    {
        sprite.loadSprite(PATH_WEAK_MONSTER, MONSTER_SPRITE_RES);
        damage = 0.5;
        speed = 2;
        health = 20;
    }
    else {
        sprite.loadSprite(PATH_STRONG_MONSTER, MONSTER_SPRITE_RES);
        damage = 1.2;
        speed = 3;
        health = 50;
    }

    while (true) {
        i = rand() % map.getLevelSize();
        j = rand() % map.getLevelSize();

        newPos = {
            float(j + 0.5) * map.getCellSize(),
            float(i + 0.5) * map.getCellSize()
        };

        if (!map.getLevelPlan()[i][j] &&
            sqrt(sqrDistance(newPos, plPos)) >
            minDistToPlayer) {
            position = newPos;
            return;
        }
    }
}

int Monster::getMonstersLeft()
{
    return monsterLeft;
}

bool Monster::isPointReached(Map& map)
{
    Vector2f NodeCentre = {
        float(pathToPlayer[0].x * map.getCellSize() + map.getCellSize() / 2),
        float(pathToPlayer[0].y * map.getCellSize() + map.getCellSize() / 2)
    };

    return speed * speed >= sqrDistance(NodeCentre, position);
}
