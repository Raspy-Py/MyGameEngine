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

void Player::listenKeyboard()
{
    if      (Keyboard::isKeyPressed(Keyboard::W)) { moveForward(); } // рухатись вперед
    else if (Keyboard::isKeyPressed(Keyboard::S)) { moveBackward(); } // рухатись назад

    if      (Keyboard::isKeyPressed(Keyboard::A)) { turnLeft(); } // повернути наліво
    else if (Keyboard::isKeyPressed(Keyboard::D)) { turnRight(); } // повернути направо

    if      (Keyboard::isKeyPressed(Keyboard::P)) { getInfo(); } // інформація для налагодження
}

void Player::takeDamage(int damage)
{
    health -= damage;
}

void Player::moveForward()
{
    position.x += speed * cos(direction);
    position.y += speed * sin(direction);
}

void Player::moveBackward()
{
    position.x -= speed * cos(direction);
    position.y -= speed * sin(direction);
}

void Player::turnLeft()
{
    direction -= rotationSpeed;
    if (direction < 0)
    {
        direction += 2 * PI;
    }
}

void Player::turnRight()
{
    direction += rotationSpeed;
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