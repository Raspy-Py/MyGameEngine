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

void Player::listenKeyboard(FPS& fps)
{
    if      (Keyboard::isKeyPressed(Keyboard::W)) { moveForward(fps); } // рухатись вперед
    else if (Keyboard::isKeyPressed(Keyboard::S)) { moveBackward(fps); } // рухатись назад

    if      (Keyboard::isKeyPressed(Keyboard::A)) { turnLeft(fps); } // повернути наліво
    else if (Keyboard::isKeyPressed(Keyboard::D)) { turnRight(fps); } // повернути направо

    if      (Keyboard::isKeyPressed(Keyboard::P)) { getInfo(); } // інформація для налагодження
}

void Player::takeDamage(int damage)
{
    health -= damage;
}

void Player::moveForward(FPS& fps)
{
    cout << fps.getFPS() << endl;
    position.x += speed * cos(direction) * fps.getFPS();
    position.y += speed * sin(direction) * fps.getFPS();
}

void Player::moveBackward(FPS& fps)
{
    position.x -= speed * cos(direction) * fps.getFPS();
    position.y -= speed * sin(direction) * fps.getFPS();
}

void Player::turnLeft(FPS& fps)
{
    direction -= rotationSpeed;
    if (direction < 0)
    {
        direction += 2 * PI;
    }
}

void Player::turnRight(FPS& fps)
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