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

    playerHealthIndicator.resize(8);
    playerHealthIndicator.setPrimitiveType(Quads);

    playerHealthIndicator[0].position = Vector2f(float(WIN_WIDTH * 3) / 4, float(WIN_HEIGHT) / 18);
    playerHealthIndicator[5].position = Vector2f(float(WIN_WIDTH * 31) / 32, float(WIN_HEIGHT) / 18);
    playerHealthIndicator[3].position = Vector2f(float(WIN_WIDTH * 3) / 4, float(WIN_HEIGHT) / 9);
    playerHealthIndicator[6].position = Vector2f(float(WIN_WIDTH * 31) / 32, float(WIN_HEIGHT) / 9);

    for (int i = 0; i < 4; i++)
        playerHealthIndicator[i].color = Color(255, 50, 50);

    for (int i = 4; i < 8; i++)
        playerHealthIndicator[i].color = Color(102, 20, 20);
}

Game::~Game()
{
}

void Game::runGame()  
{
     ContextSettings settings;
     //settings.antialiasingLevel = 8;

     // Створюємо вікно
     window.create(VideoMode(WIN_WIDTH - 1, WIN_HEIGHT), "RayCastingGameEngine", Style::Titlebar | Style::Close, settings);
     //window.setVerticalSyncEnabled(true);


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
        case Game::END_GAME:
            endgameMenu();
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
    int mouseDelta;
    int mouseOldX = WIN_HALF_WIDTH;
    int mouseNewX;
    Vector2i centre(WIN_HALF_WIDTH, WIN_HALF_HEIGHT);

    RayCasting rc;
    EntitySprite monsterSprite;
    monsterSprite.loadSprite(PATH_WEAK_MONSTER, MONSTER_SPRITE_RES);

    window.setMouseCursorVisible(false);
    Mouse::setPosition(centre, window);
    CircleShape pointer(POINTER_RADIUS, 8);

    pointer.setOrigin(POINTER_RADIUS, POINTER_RADIUS);
    pointer.setPosition(WIN_HALF_WIDTH, WIN_HALF_HEIGHT);
    pointer.setOutlineThickness(1.5);
    pointer.setFillColor(Color(0,0,0,0));
    pointer.setOutlineColor(Color(150,150,150));

    FPS fps;
    Player player(map.levelSize, map.cellSize);
    Monster monster_1;

    monster_1.respawn(map, player.getPosition(), monsterSprite);

    Text monstersLeftText;
    monstersLeftText.setFont(menuFont);
    monstersLeftText.setString(to_string(NUMBER_OF_MONSTERS) +
    "/" + to_string(NUMBER_OF_MONSTERS));
    monstersLeftText.setCharacterSize(20);
    monstersLeftText.setPosition(float(WIN_WIDTH * 31)/32 - 
        (float)monstersLeftText.getString().getSize() * 0.83 * monstersLeftText.getCharacterSize(),
        float(WIN_HEIGHT)/9 + 4);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        

        ////---Обробка керування мишею---//
        mouseNewX = Mouse::getPosition(window).x;
        Mouse::setPosition(centre, window);
        mouseDelta = mouseNewX - mouseOldX;

        if (Mouse::isButtonPressed(Mouse::Button::Left)) {
            player.shoot(monster_1, rc.raysLength);
        }

        player.rotateByMouse(fps, mouseDelta);
        player.listenKeyboard(fps, map, rc.raysLength, monster_1);
        monster_1.makeStep(fps, map, player);
        rc.castRays(player, map);
        map.updateMinimap(player,monster_1, rc.raysEndCords);
        render.updateImage(rc);
        monsterSprite.calculateSprite(player, monster_1.getPosition());
        monster_1.checkStatus(player, map, monsterSprite);
        updateIndicator(player.getHealth());
        fps.updateFPS();

        runThis = checkGameStatus(player, monster_1);

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            background.setColor(Color(255, 255, 255, 255));
            runThis = false;
            toDisplay = MAIN_MENU;
            unloadTemplates();
            render.deleteTexture();
        }

        monstersLeftText.setString(to_string(monster_1.monsterLeft) +
            "/" + to_string(NUMBER_OF_MONSTERS));
        monstersLeftText.setPosition(float(WIN_WIDTH * 31) / 32 -
            (float)monstersLeftText.getString().getSize() * 0.83 * monstersLeftText.getCharacterSize(),
            float(WIN_HEIGHT) / 9 + 4);

        window.clear();
        render.draw(window);
        monsterSprite.draw(window, rc);
        map.draw(window);
        window.draw(pointer);
        window.draw(playerHealthIndicator);
        window.draw(monstersLeftText);
        fps.draw(window);
        window.display();
    }


    window.setMouseCursorVisible(true);
}

void Game::mainMenu()
{
    Clock time;
    Time elapsed;
    time.restart();

    map.clearInfo();

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
            }
            else if (createLevelButton.isPressed(window)) {
                elapsed = time.getElapsedTime();
                if (elapsed.asMilliseconds() > 200) {
                    background.setColor(Color(255, 255, 255, 84));
                    runThis = false;
                    toDisplay = LEVEL_CREATION_MENU;
                    time.restart();
                }

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
    Clock time;
    Time elapsed;
    time.restart();


    bool runThis = true;
    FPS fps;

    int levelToGenerateSize = 0;

    //---Кнопки---//
    Button generateButton(Vector2f(float(WIN_WIDTH * 3) / 8, float(WIN_HEIGHT * 19) / 36), Vector2f(float(WIN_WIDTH) / 4, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));
    Button minusButton(Vector2f(float(WIN_WIDTH * 3) / 8, float(WIN_HEIGHT * 13) / 36), Vector2f(float(WIN_WIDTH) / 16, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));
    Button plusButton(Vector2f(float(WIN_WIDTH * 9) / 16, float(WIN_HEIGHT * 13) / 36), Vector2f(float(WIN_WIDTH) / 16, float(WIN_HEIGHT) / 9), Color(255, 255, 255, 0));
    Button back(Vector2f(0, 0), Vector2f(float(WIN_WIDTH * 3) / 32, float(WIN_HEIGHT) / 18), Color(255, 255, 255, 0));

    minusButton.setTextFont(menuFont);
    minusButton.setTextString("-");
    minusButton.setTextSize(30);
    minusButton.setTextColor(Color(255, 255, 255, 255));
    minusButton.alignTextCentre(FONT_AR);

    plusButton.setTextFont(menuFont);
    plusButton.setTextString("+");
    plusButton.setTextSize(30);
    plusButton.setTextColor(Color(255, 255, 255, 255));
    plusButton.alignTextCentre(FONT_AR);

    generateButton.setTextFont(menuFont);
    generateButton.setTextString("Generate ");
    generateButton.setTextSize(30);
    generateButton.setTextColor(Color(255, 255, 255, 255));
    generateButton.alignTextCentre(FONT_AR);

    back.setTextFont(menuFont);
    back.setTextString("back");
    back.setTextSize(20);
    back.setTextColor(Color(200, 200, 200, 200));
    back.alignTextCentre(FONT_AR);

    //---Написи---//
    Text genLevelSizeText;
    genLevelSizeText.setCharacterSize(30);
    genLevelSizeText.setFont(menuFont);
    genLevelSizeText.setString("15");
    genLevelSizeText.setPosition(float(WIN_HALF_WIDTH) - 30.0 * FONT_AR, float(WIN_HEIGHT * 15) / 36 - 15.0);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();


        //---Обробка натискань на кнопки---//
        if (minusButton.isPressed(window)) {

            elapsed = time.getElapsedTime();

            if (elapsed.asMilliseconds() > 85) {
                levelToGenerateSize-=2;
                if (levelToGenerateSize < 0) levelToGenerateSize = 50;

                genLevelSizeText.setString(to_string(15 + levelToGenerateSize));
                time.restart();
            }
        }
        else if (plusButton.isPressed(window)) {
            elapsed = time.getElapsedTime();

            if (elapsed.asMilliseconds() > 85) {
                levelToGenerateSize += 2;
                if (levelToGenerateSize >50 ) levelToGenerateSize = 0;


                genLevelSizeText.setString(to_string(15 + levelToGenerateSize));
                time.restart();
            }
        }
        else if (generateButton.isPressed(window)) {     
            elapsed = time.getElapsedTime();

            if (elapsed.asMilliseconds() > 200) {
                render.init("../data/textures/brick.bmp", Color(98, 107, 67), Color(56, 61, 38));
                map.generateMap(15 + levelToGenerateSize, MINIMAP_SIZE);

                toDisplay = START_LEVEL;
                runThis = false;
                time.restart();
            }
        }
        else if (back.isPressed(window)) {
            runThis = false;
            toDisplay = MAIN_MENU;

        }
       

        fps.updateFPS();

        window.clear();
        window.draw(background);
        minusButton.draw(window);
        plusButton.draw(window);
        generateButton.draw(window);
        back.draw(window);
        window.draw(genLevelSizeText);
        fps.draw(window);
        window.display();
    }
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
    Button back(Vector2f(0, 0), Vector2f(float(WIN_WIDTH * 3) / 32, float(WIN_HEIGHT) / 18), Color(255, 255, 255, 0));

    prevTemplate.setTextFont(menuFont);
    prevTemplate.setTextString("<<");

    nextTemplate.setTextFont(menuFont);
    nextTemplate.setTextString(">>");

    launchMap.setTextFont(menuFont);
    launchMap.setTextString("Play");

    back.setTextFont(menuFont);
    back.setTextString("back");
    back.setTextSize(20);
    back.setTextColor(Color(200,200,200,200));

    launchMap.alignTextCentre(FONT_AR);
    nextTemplate.alignTextCentre(FONT_AR);
    prevTemplate.alignTextCentre(FONT_AR);
    back.alignTextCentre(FONT_AR);

    while (window.isOpen() && runThis) {

        // Обробка подій
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        

        //---Обробка натискань на кнопки---//
        if (back.isPressed(window)) {
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
        back.draw(window);
        fps.draw(window);
        window.display();
    }
}

void Game::endgameMenu()
{

    bool runThis = true;

    FPS fps;

    gameResultText.setFont(menuFont);

    Button restartButton(Vector2f(WIN_HALF_WIDTH - 135, WIN_HALF_HEIGHT + 50), Vector2f(270, 40), Color(255, 255, 255, 0));
    restartButton.setTextString("Restart");
    restartButton.setTextColor(Color(255, 255, 255));
    restartButton.setTextFont(menuFont);
    restartButton.setTextPosition(Vector2f(WIN_HALF_WIDTH - 105, WIN_HALF_HEIGHT + 50));
    restartButton.alignTextCentre(FONT_AR);

    Button mainMenuButton(Vector2f(WIN_HALF_WIDTH - 135, WIN_HALF_HEIGHT - 10), Vector2f(270, 40), Color(255, 255, 255, 0));
    mainMenuButton.setTextString("Main menu");
    mainMenuButton.setTextColor(Color(255, 255, 255));
    mainMenuButton.setTextFont(menuFont);
    mainMenuButton.alignTextCentre(FONT_AR);


    if (!backgroundTexture.loadFromFile(PATH_MM_BACKGROUND))
        std::cout << "Failed to load background for main menu! " << std::endl;
    background.setTexture(backgroundTexture);
    background.setPosition(Vector2f(0, 0));

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


        if (restartButton.isPressed(window)) {
            runThis = false;
            toDisplay = START_LEVEL;
        }
        else if (mainMenuButton.isPressed(window)) {
            background.setColor(Color(255, 255, 255, 255));
            runThis = false;
            toDisplay = MAIN_MENU;
        }

        

        fps.updateFPS();

        window.clear();
        window.draw(background);
        fps.draw(window);
        window.draw(gameResultText);
        restartButton.draw(window);
        mainMenuButton.draw(window);
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

bool Game::checkGameStatus(Player& player, Monster& monster) {
    if (player.getHealth() <= 0) {
        gameResultText.setCharacterSize(40);
        gameResultText.setString("You loose!");
        gameResultText.setPosition(
            (float)WIN_HALF_WIDTH - (float)gameResultText.getString().getSize() * FONT_AR * gameResultText.getCharacterSize() / 2,
            float(WIN_HEIGHT) / 3);

        toDisplay = END_GAME;
        background.setColor(Color(255, 255, 255, 84));
        return false;
    }
    else if (monster.monsterLeft <= 0) {
        gameResultText.setCharacterSize(40);
        gameResultText.setString("You win!");
        gameResultText.setPosition(
            (float)WIN_HALF_WIDTH - (float)gameResultText.getString().getSize() * FONT_AR * gameResultText.getCharacterSize() / 2,
            float(WIN_HEIGHT) / 3);

        toDisplay = END_GAME;
        background.setColor(Color(255, 255, 255, 84));
        return false;
    }

    return true;
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

    file.close();
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

    //---Налаштування перегляду назви рівня---//

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
        targetSize.y / wallSpritePreview.getLocalBounds().height
    );


    //---Налаштовуємо перегляд кольорів підлоги та стелі---//

    floorPreview.setFillColor(floorColor[tempIndex]);
    ceilingPreview.setFillColor(ceilingColor[tempIndex]);
}

void Game::updateIndicator(float hp)
{
    playerHealthIndicator[1].position = Vector2f(float(2400.0  + hp * 7) * WIN_WIDTH / 3200, float(WIN_HEIGHT) / 18);
    playerHealthIndicator[4].position = Vector2f(float(2400.0  + hp * 7) * WIN_WIDTH / 3200, float(WIN_HEIGHT) / 18);
    playerHealthIndicator[2].position = Vector2f(float(2400.0  + hp * 7) * WIN_WIDTH / 3200, float(WIN_HEIGHT) / 9);
    playerHealthIndicator[7].position = Vector2f(float(2400.0  + hp * 7) * WIN_WIDTH / 3200, float(WIN_HEIGHT) / 9);
}
