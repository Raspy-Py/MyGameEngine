#include "Player.h"

Player::Player(float x, float y)
{

    speed = PLAYER_DEFAULT_SPEED;
    strafeSpeed = STRAFE_SPEED;
    speedInStrafe = sqrt(speed * speed - strafeSpeed * strafeSpeed);
    rotationSpeed = PLAYER_DEFAULT_ROTATION_SPEED;
    health = PLAYER_DEFAULT_HEALTH;
    colBoxSize = PLAYER_COLISION_BOX_SIZE;
    direction = PI;
    position.x = x;
    position.y = y;

}

Player::~Player()
{
}

void Player::listenKeyboard(FPS& fps, Map& map)
{
    speed = PLAYER_DEFAULT_SPEED;

    if      (Keyboard::isKeyPressed(Keyboard::A)) { strafeLeft(fps, map); }
    else if (Keyboard::isKeyPressed(Keyboard::D)) { strafeRight(fps, map); }
    
    if      (Keyboard::isKeyPressed(Keyboard::W)) { moveForward(fps, map); } 
    else if (Keyboard::isKeyPressed(Keyboard::S)) { moveBackward(fps, map); } 


    if      (Keyboard::isKeyPressed(Keyboard::P)) { getInfo(); } // TODO: remove
}

void Player::takeDamage(int damage)
{
    health -= damage;
}

void Player::rotateByMouse(FPS& fps, int delta)
{
    direction += limitRotationSpeed(delta) * MOUSE_SENS * fps.getFPS();

    if (direction < 0)
        direction += 2 * PI;
    else if (direction > PI * 2)
        direction -= PI * 2;
}

void Player::moveForward(FPS& fps, Map& map)
{

    float newX = position.x + speed * cos(direction) * fps.getFPS();
    float newY = position.y + speed * sin(direction) * fps.getFPS();

    int oldQuadX = int(position.x) / map.cellSize;
    int oldQuadY = int(position.y) / map.cellSize;

    if (!isWallColided(newX, newY, map))
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!isWallColided(position.x, newY, map))
    {
        position.y = newY;
    }
    else if (!isWallColided(newX, position.y, map))
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

    if (!isWallColided(newX, newY, map))
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!isWallColided(position.x, newY, map))
    {
        position.y = newY;
    }
    else if (!isWallColided(newX, position.y, map))
    {
        position.x = newX;
    }
}

void Player::strafeRight(FPS& fps, Map& map)
{
    float newX = position.x + strafeSpeed * cos(direction + ANGLE_90) * fps.getFPS();
    float newY = position.y + strafeSpeed * sin(direction + ANGLE_90) * fps.getFPS();

    int oldQuadX = int(position.x) / map.cellSize;
    int oldQuadY = int(position.y) / map.cellSize;

    if (!isWallColided(newX, newY, map))
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!isWallColided(position.x, newY, map))
    {
        position.y = newY;
    }
    else if (!isWallColided(newX, position.y, map))
    {
        position.x = newX;
    }

    speed = speedInStrafe;
}

void Player::strafeLeft(FPS& fps, Map& map)
{
    float newX = position.x + strafeSpeed * cos(direction - ANGLE_90) * fps.getFPS();
    float newY = position.y + strafeSpeed * sin(direction - ANGLE_90) * fps.getFPS();

    int oldQuadX = int(position.x) / map.cellSize;
    int oldQuadY = int(position.y) / map.cellSize;

    if (!isWallColided(newX, newY, map))
    {
        position.x = newX;
        position.y = newY;
    }
    else if (!isWallColided(position.x, newY, map))
    {
        position.y = newY;
    }
    else if (!isWallColided(newX, position.y, map))
    {
        position.x = newX;
    }

    speed = speedInStrafe;
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
    cout << "<----Information about player---->" << endl;

    cout << "Position: (" <<position.x << ", "<< position.y << ")" << endl;
    cout << "Direction: " << direction/ PI * 180  << endl;
    cout << "Health: " << health << endl;
    cout << "Speed: " << speed << " units/tick" << endl;
    cout << "Ratation speed: " << rotationSpeed / PI * 180 <<" grad/tick" <<  endl;

    cout << "<-------------------------------->" << endl;
}

bool Player::isWallColided(float plX, float plY, Map& map)
{
    int i0, i1;
    int j0, j1;

    j0 = int(plX - colBoxSize) / map.cellSize;
    j1 = int(plX + colBoxSize) / map.cellSize;
    i0 = int(plY - colBoxSize) / map.cellSize;
    i1 = int(plY + colBoxSize) / map.cellSize;

    if (map.levelPlan[i0][j0])
        return true;
    else if (map.levelPlan[i0][j1])
        return true;
    else if (map.levelPlan[i1][j0])
        return true;
    else if (map.levelPlan[i1][j1])
        return true;

    return false;
}

int Player::limitRotationSpeed(int delta)
{
    if (delta > 0)
        if (delta > MAX_MOUSE_DELTA)
            return MAX_MOUSE_DELTA;

    if (delta < 0)
        if (delta < -MAX_MOUSE_DELTA)
            return -MAX_MOUSE_DELTA;

    return delta;
}
