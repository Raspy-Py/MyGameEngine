#include <iostream>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "RayCasting.h"

using namespace std;

template <typename T>
void print_matrix(T** arr, int n); // Функція для налагодження

int main()
{
    Map map(MINIMAP_SIZE, "../data/levels/level.csv");
    Player player(0, 0);
    RayCasting rc;

    ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Створюємо вікно
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "RayCastingGameEngine", Style::Default, settings);
    window.setFramerateLimit(60);

    print_matrix(map.levelPlan, map.levelSize);

    while (window.isOpen()) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        
        player.listenKeyboard();
        rc.castRays(player, map);
        map.updateMinimap(player, rc.raysEndCords);

        window.clear();
        map.draw(window);
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