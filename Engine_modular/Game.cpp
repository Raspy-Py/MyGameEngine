#include "Game.h"

Game::Game()
{
    //---Стартове вікно за замовченням---//
    toDisplay = MAIN_MENU;

    //---Завантажуємо шрифт для усієї гри---//
    if (!menuFont.loadFromFile(PATH_DEFAULT_FONT)) {
        std::cout << "Failed to load font for menu!" << std::endl;
    }

    //---Сталі налаштування попереднього перегляду рівня---//
    levelPreview.setPrimitiveType(Quads);

    floorPreview.setSize(Vector2f(float(WIN_WIDTH * 3) / 16, float(WIN_HALF_HEIGHT) / 3));
    ceilingPreview.setSize(Vector2f(float(WIN_WIDTH * 3)/16, float(WIN_HALF_HEIGHT) / 3));

    texturePreviewPos = { float(3 * WIN_WIDTH) / 32, float(WIN_HEIGHT) / 3 };
    floorPreviewPos = { float(23 * WIN_WIDTH) / 32, WIN_HALF_HEIGHT };
    ceilingPreviewPos = { float(23 * WIN_WIDTH) / 32, float(WIN_HEIGHT) / 3 };
    mapPreviewPos = {float(11 * WIN_WIDTH) / 32, float(2 * WIN_HEIGHT) / 9};

    wallSpritePreview.setPosition(texturePreviewPos);
    floorPreview.setPosition(floorPreviewPos);
    ceilingPreview.setPosition(ceilingPreviewPos);

    namePreview.setCharacterSize(60);
    namePreview.setFont(menuFont);
    namePreview.setFillColor(Color(255,255,255));
}

Game::~Game()
{
}

void Game::runGame()  
{
     ContextSettings settings;
     //settings.antialiasingLevel = 8;

     // Створюємо вікно
     window.create(VideoMode(WIN_WIDTH - 1, WIN_HEIGHT), "RayCastingGameEngine", Style::Default, settings);

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

    RayCasting rc;
    
    FPS fps;
    Player player(100, 100);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            background.setColor(Color(255, 255, 255, 255));
            runThis = false;
            toDisplay = MAIN_MENU;
            unloadTemplates();
            render.deleteTexture();
        }

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

    Button loadLevelButton(Vector2f(WIN_HALF_WIDTH - 135,WIN_HALF_HEIGHT + 50), Vector2f(270, 40), Color(255, 255, 255,0));
    loadLevelButton.setTextString("Load level");
    loadLevelButton.setTextColor(Color(255,255,255));
    loadLevelButton.setTextFont(menuFont);
    loadLevelButton.setTextPosition(Vector2f(WIN_HALF_WIDTH - 105, WIN_HALF_HEIGHT + 50));

    Button createLevelButton(Vector2f(WIN_HALF_WIDTH - 135, WIN_HALF_HEIGHT - 10), Vector2f(270, 40), Color(255, 255, 255, 0));
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
    

    //---Оновлюємо задній фон---//
    background.setColor(Color(255, 255, 255, 255));


    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        

        if (loadLevelButton.isPressed(window)) {
            background.setColor(Color(255, 255, 255, 84));
            runThis = false;
            toDisplay = LEVEL_CHOOSE_MENU;
        }else if(createLevelButton.isPressed(window)) {
            background.setColor(Color(255,255,255,84));
            //runThis = false;
            //toDisplay = START_LEVEL;
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
    Clock time;
    Time elapsed;
    time.restart();


    bool runThis = true;
    int chosenTemplate = 0;
    FPS fps;

    loadTemplates();
    switchTemplate(0);


    //---Кнопки---//

    Button prevTemplate(Vector2f(float(WIN_WIDTH) / 8, float(5 * WIN_HEIGHT) / 6), Vector2f(float(WIN_WIDTH) / 8, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));
    Button nextTemplate(Vector2f(float(WIN_WIDTH * 3) / 4, float(5 * WIN_HEIGHT) / 6), Vector2f(float(WIN_WIDTH) / 8, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));
    Button launchMap(Vector2f(float(WIN_WIDTH * 13) / 32, float(5 * WIN_HEIGHT) / 6), Vector2f(float(WIN_WIDTH * 3) / 16, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));

    prevTemplate.setTextFont(menuFont);
    prevTemplate.setTextString("<<");

    nextTemplate.setTextFont(menuFont);
    nextTemplate.setTextString(">>");

    launchMap.setTextFont(menuFont);
    launchMap.setTextString("Play");

    launchMap.alignTextCentre(FONT_AR);
    nextTemplate.alignTextCentre(FONT_AR);
    prevTemplate.alignTextCentre(FONT_AR);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();


        //---Обробка натискань на кнопки---//
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            runThis = false;
            toDisplay = MAIN_MENU;
            unloadTemplates();
        }
        else if (prevTemplate.isPressed(window))
        {
            elapsed = time.getElapsedTime();

            if (elapsed.asMilliseconds() > MIN_ELAPSED)
            {
            chosenTemplate--;
            if (chosenTemplate == -1) { chosenTemplate = levelName.size() - 1; }
            switchTemplate(chosenTemplate);
            time.restart();
            }
        }
        else if (nextTemplate.isPressed(window))
        {
            elapsed = time.getElapsedTime();

            if (elapsed.asMilliseconds() > MIN_ELAPSED)
            {
                chosenTemplate = (++chosenTemplate) % levelName.size();
                switchTemplate(chosenTemplate);
                time.restart();
            }
        }
        else if (launchMap.isPressed(window)) 
        {
            render.init(wallTexturePath[chosenTemplate], floorColor[chosenTemplate], ceilingColor[chosenTemplate]);
            map.loadMap(MINIMAP_SIZE, levelPlanPath[chosenTemplate]);
            runThis = false;
            toDisplay = START_LEVEL;
        }

        fps.updateFPS();

        window.clear();
        window.draw(background);
        window.draw(wallSpritePreview);
        window.draw(levelPreview);
        window.draw(ceilingPreview);
        window.draw(floorPreview);
        window.draw(namePreview);
        nextTemplate.draw(window);
        prevTemplate.draw(window);
        launchMap.draw(window);
        fps.draw(window);
        window.display();
    }
}

Color& Game::stringToColor(std::string str)
{
    Color rgbColor;

    rgbColor.r = stoi(str.substr(0, 2).c_str(), 0, 16);
    rgbColor.g = stoi(str.substr(2, 2).c_str(), 0, 16);
    rgbColor.b = stoi(str.substr(4, 2).c_str(), 0, 16);

    return rgbColor;
}

void Game::loadTemplates()
{
    ifstream file(PATH_TEMPLATES);
    std::string buff;

    if (!file.is_open())
    {
        std::cout << "Failed to load templates file!" << std::endl;
    }

    while (!file.eof())
    {
        Color fl;
        Color cl;

        std::getline(file, buff, ';');
        levelName.push_back(buff);

        std::getline(file, buff, ';');
        levelPlanPath.push_back(buff);

        std::getline(file, buff, ';');
        wallTexturePath.push_back(buff);

        std::getline(file, buff, ';');
        cl = stringToColor(buff);
        ceilingColor.push_back(cl);

        std::getline(file, buff, '\n');
        fl = stringToColor(buff);
        floorColor.push_back(fl);
    }
}

void Game::unloadTemplates()
{
    floorColor.clear();
    ceilingColor.clear();
    levelName.clear();
    levelPlanPath.clear();
    wallTexturePath.clear();
}

void Game::switchTemplate(int tempIndex)  
{
    ////////////////////////////////////////////////
    //Налаштовуємо попередній перегляд схеми рівня//
    ////////////////////////////////////////////////

    //---Налаштування перегляду назви рінвя---//

    namePreview.setString(levelName[tempIndex]);

    float nameLen = float(namePreview.getString().getSize()) * namePreview.getCharacterSize() * FONT_AR/2;
    namePreview.setPosition(
        WIN_HALF_WIDTH - nameLen,
        float(WIN_HEIGHT) / 18
    );

    //---Завантаження схеми рівня---//

    ifstream file(levelPlanPath[tempIndex]);
    std::string nStr;
    std::string cellStr;
    int cell;
    int previewCellSize;

    std::getline(file, nStr, '\n');
    levelPreviewSize = stoi(nStr);

    std::getline(file, nStr, '\n');
    previewCellSize = stoi(nStr);

    if (!file.is_open()) {
        std::cout << "Failed to load level plan for preview!" << std::endl;
    }

    levelPreviewPlan = new int* [levelPreviewSize];

    for (int i = 0; i < levelPreviewSize; i++)
    {
        levelPreviewPlan[i] = new int[levelPreviewSize];

        for (int j = 0; j < levelPreviewSize - 1; j++)
        {
            std::getline(file, cellStr, ',');
            cell = stoi(cellStr);
            levelPreviewPlan[i][j] = cell;
        }
        std::getline(file, cellStr, '\n');
        cell = stoi(cellStr);
        levelPreviewPlan[i][levelPreviewSize - 1] = cell;
    }
    file.close();


    //---Налаштування масиву вершин---//

    levelPreview.resize(levelPreviewSize * levelPreviewSize * 4);

    Color cellColor;

    float cellX;
    float cellY;

    float cellPreviewSizeH = float(5 * WIN_WIDTH)/16 / levelPreviewSize;
	float cellPreviewSizeV = float(5 * WIN_HEIGHT)/9 / levelPreviewSize;

    for (int i = 0; i < levelPreviewSize; i++)
    {
        for (int j = 0; j < levelPreviewSize; j++)
        {

            cellX = mapPreviewPos.x + cellPreviewSizeH * j;
            cellY = mapPreviewPos.y + cellPreviewSizeV * i;

            levelPreview[(i * levelPreviewSize + j) * 4 + 0].position = Vector2f(cellX, cellY);
            levelPreview[(i * levelPreviewSize + j) * 4 + 1].position = Vector2f(cellX + cellPreviewSizeH, cellY);
            levelPreview[(i * levelPreviewSize + j) * 4 + 2].position = Vector2f(cellX + cellPreviewSizeH, cellY + cellPreviewSizeV);
            levelPreview[(i * levelPreviewSize + j) * 4 + 3].position = Vector2f(cellX, cellY + cellPreviewSizeV);

            if (levelPreviewPlan[i][j])
                cellColor = Color(255,255,255,255); // Є стіна
            else
                cellColor = Color(255, 255, 255, 0); // Немає стіни

            levelPreview[(i * levelPreviewSize + j) * 4 + 0].color = cellColor;
            levelPreview[(i * levelPreviewSize + j) * 4 + 1].color = cellColor;
            levelPreview[(i * levelPreviewSize + j) * 4 + 2].color = cellColor;
            levelPreview[(i * levelPreviewSize + j) * 4 + 3].color = cellColor;
        }
    }


    //---Налаштовуємо перегляд текстури---//

    if (!wallTexturePreview.loadFromFile(wallTexturePath[tempIndex]))
        std::cout << "Failed to load preview level texture!" << std::endl;

    wallSpritePreview.setTexture(wallTexturePreview);

    Vector2f targetSize(float(WIN_WIDTH * 3) / 16, float(WIN_HEIGHT) / 3);
    wallSpritePreview.setScale(
        targetSize.x / wallSpritePreview.getLocalBounds().width,
        targetSize.y / wallSpritePreview.getLocalBounds().height);


    //---Налаштовуємо перегляд кольорів підлоги та стелі---//

    floorPreview.setFillColor(floorColor[tempIndex]);
    ceilingPreview.setFillColor(ceilingColor[tempIndex]);
}
