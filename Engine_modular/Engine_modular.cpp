#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"
#include "Render.h"
#include "FPSHandler.h"

using namespace std;

template <typename T>
void print_matrix(T** arr, int n); // Функція для налагодження

int main()
{
    Map map(MINIMAP_SIZE, PATH_LEVEL);
    RayCasting rc;
    RenderImage render(PATH_WALL_TEXTURE);
    FPS fps;
    Player player(100, 100);

    ContextSettings settings;
    //settings.antialiasingLevel = 8;

    // Створюємо вікно
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "RayCastingGameEngine", Style::Default/*, settings*/);
    //window.setFramerateLimit(60);



    while (window.isOpen()) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        
        player.listenKeyboard(fps);
        rc.castRays(player, map);
        map.updateMinimap(player, rc.raysEndCords);
        render.updateImage(rc);
        fps.updateFPS();

        window.clear();
        render.draw(window);
        map.draw(window);
        fps.draw(window);
        window.display();
    }
}

template <typename T>
void print_matrix(T** arr, int n) {
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}