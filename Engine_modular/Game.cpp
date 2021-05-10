#include "Game.h"

Game::Game()
{
    toDisplay = 0;

    if (!menuFont.loadFromFile(PATH_DEFAULT_FONT)) {
        std::cout << "Failed to load font for menu!" << std::endl;
    }
}

Game::~Game()
{
}

void Game::runGame()
{
     ContextSettings settings;
     settings.antialiasingLevel = 8;

     // Створюємо вікно
     window.create(VideoMode(WIN_WIDTH, WIN_HEIGHT), "RayCastingGameEngine", Style::Default, settings);

     toDisplay = MAIN_MENU;

    while (window.isOpen()) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        switch (toDisplay)
        {
        case Game::MAIN_MENU:
            mainMenu();
            break;
        case Game::LEVEL_CREATION_MENU:
            levelCreationMenu();
            break;
        case Game::LEVEL_CHOOSE_MENU:
            levelChooseMenu();
            break;
        case Game::START_LEVEL:
            startLevel();
            break;
        default:
            std::cout << "Wrong displaying mode chosen: " << toDisplay << std::endl;
            std::cout << "Switched to MAIN_MENU (0)" << std::endl;
            toDisplay = MAIN_MENU;
            break;
        }

    }
}

void Game::startLevel()
{
    bool runThis = true;

    Map map(MINIMAP_SIZE, PATH_LEVEL);
    RayCasting rc;
    RenderImage render(PATH_WALL_TEXTURE);
    FPS fps;
    Player player(100, 100);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed ||
                Keyboard::isKeyPressed(Keyboard::Escape))
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
    bool runThis = true;

    FPS fps;

    Button loadLevelButton(Vector2f(WIN_HALF_WIDTH - 135,WIN_HALF_HEIGHT+40 + 10), Vector2f(270, 40), Color(201, 151, 0,10));
    loadLevelButton.setTextString("Load level");
    loadLevelButton.setTextColor(Color(255,255,255));
    loadLevelButton.setTextFont(menuFont);
    loadLevelButton.setTextPosition(Vector2f(WIN_HALF_WIDTH - 105, WIN_HALF_HEIGHT + 50));

    Button createLevelButton(Vector2f(WIN_HALF_WIDTH - 135, WIN_HALF_HEIGHT - 10), Vector2f(270, 40), Color(201, 151, 0, 10));
    createLevelButton.setTextString("Create level");
    createLevelButton.setTextColor(Color(255, 255, 255));
    createLevelButton.setTextFont(menuFont);

    
    if (!backgroundTexture.loadFromFile(PATH_MM_BACKGROUND))
        std::cout << "Failed to load background for main menu! " << std::endl;
    background.setTexture(backgroundTexture);
    background.setPosition(Vector2f(0,0));

    //---Масштабуємо спрайт фону в меню---///
    Vector2f targetSize(WIN_WIDTH, WIN_HEIGHT);
    background.setScale(
        targetSize.x / background.getLocalBounds().width,
        targetSize.y / background.getLocalBounds().height);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        

        if (loadLevelButton.isPressed(window)) {
            background.setColor(Color(255, 255, 255, 255));
            //runThis = false;
            //toDisplay = START_LEVEL;
        }else if(createLevelButton.isPressed(window)) {
            background.setColor(Color(255,255,255,128));
            runThis = false;
            toDisplay = START_LEVEL;
        }

        fps.updateFPS();

        window.clear();
        window.draw(background);
        fps.draw(window);
        loadLevelButton.draw(window);
        createLevelButton.draw(window);
        window.display();
    }
}

void Game::levelCreationMenu()
{
}

void Game::levelChooseMenu()
{
}
