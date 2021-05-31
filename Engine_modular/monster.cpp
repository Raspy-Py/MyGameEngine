#include "Monster.h"

std::vector<Monster*> Monster::monstersList = {};

Monster::Monster()
{
	monstersList.push_back(this);
}

Monster::~Monster()
{
    monstersList.clear();
}

void Monster::makeStep(FPS& fps, Map& map, Player& player)
{
    if (pathToPlayer.size() && 
        !distance(playerLastPosition , Vector2i(
            (int)player.getPosition().x / map.cellSize, 
            (int)player.getPosition().y / map.cellSize))) {

        if (isPointReached(map)) {
            pathToPlayer.erase(pathToPlayer.begin());
        }
        else {
            float dx, dy;
            Vector2f node = {
                float(pathToPlayer[0].x * map.cellSize + map.cellSize / 2),
                float(pathToPlayer[0].y * map.cellSize + map.cellSize / 2)
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
            (int)player.getPosition().x / map.cellSize,
            (int)player.getPosition().y / map.cellSize
        };
        target = {
            (int)position.x / map.cellSize,
            (int)position.y / map.cellSize
        };

        playerLastPosition = start;
        pathToPlayer = aStarPathFind(map.levelPlan, map.levelSize, start, target);

        //makeStep(fps, map, player);
    }
}

std::vector<Monster*>& Monster::getMonstersList()
{
	return monstersList;
}

void Monster::painterSort(const Vector2f& playerPos)
{
	Monster* tempPtr;
	float tempDist;
	std::vector<float> sqrDistList;

	for (auto monster : monstersList)
		sqrDistList.push_back(sqrDistance(playerPos, monster->position));


	for (int i = 0; i < sqrDistList.size(); i++)
	{
		for (int j = 1; j < sqrDistList.size()- i; j++)
		{
			if (sqrDistList[j] > sqrDistList[j - 1])
			{
				tempDist = sqrDistList[j];
				sqrDistList[j] = sqrDistList[j - 1];
				sqrDistList[j - 1] = tempDist;

				tempPtr = monstersList[j];
				monstersList[j] = monstersList[j - 1];
				monstersList[j - 1] = tempPtr;
			}
		}
	}
}

float Monster::sqrDistance(const Vector2f& a,const Vector2f& b)
{
	return pow(a.x - b.x, 2) + pow(a.y - b.y, 2);
}

std::vector<Vector2i> Monster::aStarPathFind(int** maze, int mazeSize, const Vector2i& start, const Vector2i& target)
{
    std::vector<Vector2i> path;
    std::vector<Node*> openNodes;
    std::vector<Vector2i> deltas = { Vector2i(-1,0),Vector2i(1,0),Vector2i(0,-1),Vector2i(0,1) };
    Node* current;
    Node* neighbour;

    //---��������� �� ���������� ����---//

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

    //---��������---//
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

        //---���� 1---//
        current = chooseCurrentNode(openNodes);
        removeFromOpenAndClose(current, openNodes);


        //---���� 2---//
        if (current->gridPos == target)
            break;

        //---���� 3---//
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

    //---������� ����---//

    Node* nextNode = &graph[target.y][target.x];
    path.push_back(nextNode->gridPos);

    while (nextNode->parent)
    {
        nextNode = nextNode->parent;
        path.push_back(nextNode->gridPos);
    }

    path.erase(path.begin());

    //---������� ���'���---//
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

bool Monster::isPointReached(Map& map)
{
    Vector2f NodeCentre = {
        float(pathToPlayer[0].x * map.cellSize + map.cellSize / 2),
        float(pathToPlayer[0].y * map.cellSize + map.cellSize / 2)
    };

    return speed * speed >= sqrDistance(NodeCentre, position);
}
