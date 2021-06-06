#include "Player.h"

Player::Player(int mapSize, int cellSize)
{
    float spawnCell;

    mapSize -= 2;
    mapSize /= 2;

    if (!(mapSize & 1)) spawnCell = ++mapSize;
    else spawnCell = mapSize;

    position.x = float(spawnCell + 0.5) * cellSize;
    position.y = float(spawnCell + 0.5) * cellSize;

    speed = PLAYER_DEFAULT_SPEED;
    strafeSpeed = STRAFE_SPEED;
    speedInStrafe = sqrt(speed * speed - strafeSpeed * strafeSpeed);
    rotationSpeed = PLAYER_DEFAULT_ROTATION_SPEED;
    health = PLAYER_DEFAULT_HEALTH;
    colBoxSize = PLAYER_COLISION_BOX_SIZE;
    direction = PI;
    damage = PLAYER_DEFAULT_DAMAGE;

}

Player::~Player()
{
}

void Player::listenKeyboard(FPS& fps, Map& map, float* raysLenth, Monster& monster)
{
    speed = PLAYER_DEFAULT_SPEED;

    if      (Keyboard::isKeyPressed(Keyboard::A)) { strafeLeft(fps, map); }
    else if (Keyboard::isKeyPressed(Keyboard::D)) { strafeRight(fps, map); }
    
    if      (Keyboard::isKeyPressed(Keyboard::W)) { moveForward(fps, map); } 
    else if (Keyboard::isKeyPressed(Keyboard::S)) { moveBackward(fps, map); } 

    if (Keyboard::isKeyPressed(Keyboard::Space)) { shoot(monster, raysLenth);}
}

void Player::takeDamage(float _damage)
{
    health -= _damage;
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
    float boost = 1;
    if (Keyboard::isKeyPressed(Keyboard::LControl)) boost = 1.4;

    float newX = position.x + speed * cos(direction) * fps.getFPS() * boost;
    float newY = position.y + speed * sin(direction) * fps.getFPS() * boost;

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

void Player::shoot(Monster& monster, float* zBuffer)
{
    Vector2f P = position;
    Vector2f E = monster.getPosition();
    Vector2f PE = E - P;
    Vector2f M;
    float k1, k2, b1, b2;
    float pEM, pPE;


    // EM:
    k1 = -(PE.x / PE.y);
    b1 = (PE.x * E.x + PE.y * E.y) / PE.y;

    // PM:
    k2 = tan(direction);
    b2 = P.y - tan(direction) * P.x;

    // M:
    M.x = (b2 - b1) / (k1 - k2);
    M.y = (b2 * k1 - b1 * k2) / (k1 - k2);

    pEM = sqrt((M.x - E.x) * (M.x - E.x) + (M.y - E.y) * (M.y - E.y));
    pPE = sqrt(PE.x * PE.x + PE.y * PE.y);

    if (pEM < MONSTER_SPRITE_RES / 2 &&
        pPE < zBuffer[WIN_HALF_WIDTH] && 
        cos(direction)*PE.x + sin(direction)*PE.y >= 0){
        monster.setHealth(monster.getHealth() - damage); 
    }
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
