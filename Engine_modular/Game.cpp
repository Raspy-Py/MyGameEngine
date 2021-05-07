#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::runGame()
{
    Map map(MINIMAP_SIZE, PATH_LEVEL);
    RayCasting rc;
    RenderImage render(PATH_WALL_TEXTURE);
    FPS fps;
    Player player(100, 100);
    
	window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT), "RayCastingGameEngine", Style::Default);
	
	while (window.isOpen())
	{
		// ������� ����
		Event event;
		while (window.pollEvent(event))
			if (event.type == Event::Closed)
				window.close();



        player.listenKeyboard(fps, map);
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

void Game::mainMenu()
{
}

void Game::levelCreationMenu()
{
}

void Game::levelChoosingMenu()
{
}

void Game::gameCycle()
{
}