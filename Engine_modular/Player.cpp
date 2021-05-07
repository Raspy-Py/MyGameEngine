#include "Player.h"

Player::Player(float x, float y)
{

    speed = PLAYER_DEFAULT_SPEED;
    rotationSpeed = PLAYER_DEFAULT_ROTATION_SPEED;
    health = PLAYER_DEFAULT_HEALTH;
    direction = PI;
    position.x = x;
    position.y = y;

}

Player::~Player()
{
}

void Player::listenKeyboard(FPS& fps, Map& map)
{
    if      (Keyboard::isKeyPressed(Keyboard::W)) { moveForward(fps, map); } // рухатись вперед
    else if (Keyboard::isKeyPressed(Keyboard::S)) { moveBackward(fps, map); } // рухатись назад

    if      (Keyboard::isKeyPressed(Keyboard::A)) { turnLeft(fps); } // повернути наліво
    else if (Keyboard::isKeyPressed(Keyboard::D)) { turnRight(fps); } // повернути направо

    if      (Keyboard::isKeyPressed(Keyboard::P)) { getInfo(); } // інформація для налагодження
}

void Player::takeDamage(int damage)
{
    health -= damage;
}

void Player::moveForward(FPS& fps, Map& map)
{
    float newX = position.x + speed * cos(direction) * fps.getFPS();
    float newY = position.y + speed * sin(direction) * fps.getFPS();

    int oldQuadX = int(position.x) / map.cellSize;
    int oldQuadY = int(position.y) / map.cellSize;

    int newQuadX = int(newX) / map.cellSize;
    int newQuadY = int(newY) / map.cellSize;

    if (!map.levelPlan[newQuadY][newQuadX])
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!map.levelPlan[newQuadY][oldQuadX])
    {
        position.y = newY;
    }
    else if (!map.levelPlan[oldQuadY][newQuadX])
    {
        position.x = newX;
    }
}

void Player::moveBackward(FPS& fps, Map& map)
{
    float newX = position.x - speed * cos(direction) * fps.getFPS();
    float newY = position.y - speed * sin(direction) * fps.getFPS();

    int oldQuadX = int(position.x) / map.cellSize;
    int oldQuadY = int(position.y) / map.cellSize;

    int newQuadX = int(newX) / map.cellSize;
    int newQuadY = int(newY) / map.cellSize;

    if (!map.levelPlan[newQuadY][newQuadX])
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!map.levelPlan[newQuadY][oldQuadX])
    {
        position.y = newY;
    }
    else if (!map.levelPlan[oldQuadY][newQuadX])
    {
        position.x = newX;
    }
}

void Player::turnLeft(FPS& fps)
{
    direction -= rotationSpeed * fps.getFPS();
    if (direction < 0)
    {
        direction += 2 * PI;
    }
}

void Player::turnRight(FPS& fps)
{
    direction += rotationSpeed * fps.getFPS();
    if (direction > PI * 2)
    {
        direction -= PI * 2;
    }
}

void Player::getInfo()
{
    system("cls");
    std::cout << "<----Information about player---->" << std::endl;

    std::cout << "Position: (" <<position.x << ", "<< position.y << ")" << std::endl;
    std::cout << "Direction: " << direction/ PI * 180  << std::endl;
    std::cout << "Health: " << health << std::endl;
    std::cout << "Speed: " << speed << " units/tick" << std::endl;
    std::cout << "Ratation speed: " << rotationSpeed / PI * 180 <<" grad/tick" << std::endl;

    std::cout << "<-------------------------------->" << std::endl;
}