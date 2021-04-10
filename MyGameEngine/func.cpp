#include "func.h"

Player::Player(double x, double y, double spd) {
    cords.x = x;
    cords.y = y;
    speed = spd;
    health = 100;
}

void Player::move_player(int way) {
    // way = -1 (backward)
    // way =  1 (forward)
    cords.x += speed * way * cos(dir);
    cords.y += speed * way * sin(dir);
}

void Player::rotate_player(int side)
{
    // side = -1 (turn left)
    // side =  1 (turn right)
    dir += side * rotationSpeed;
    if (dir > PI * 2)
    {
        dir -= 2 * PI;
    }
    else if (dir < 0)
    {
        dir += 2 * PI;
    }
}

void Player::get_info()
{
    cout << "X:\t" << cords.x << endl;
    cout << "Y:\t" << cords.y << endl;
    cout << "Dir\t" << dir << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void Player::cast_rays(int** level, RenderWindow& window)
{
    float x0 = cords.x, y0 = cords.y; // Координати гравця
    float xa = 0, ya = 0; // Координати першого перетину променя із сіткою
    float dx, dy; // Крок
    double ra; // Напрямок промення в радіанах
    float tanAlpha;
    int iter = 1; // Максимальна к-ть ітерацій на промінь
    int h=0, c;
    Vector2f infty(10000,10000);

    VertexArray singleRay(Lines, WIN_WIDTH*2); // Пустимо один промінь для налагодження 
    
    
    float xh, yh,xv,yv; 

    for (double i = dir - FOV / 2; i < dir + FOV / 2 - 0.0001; i += ABR, h+=1)
    {
        c = 2 * h;

        ra = normalize_angle(i);

        singleRay[c].position = cords;
        singleRay[c+1].position = infty;
        singleRay[c].color = Color(255, 0, 0);
        singleRay[c+1].color = Color(255, 0, 0);

        // Перетин з вертикальними лініями
        tanAlpha = tan(ra);
        if (ra < P2 || ra > P3) // Промінь випущено вправо
        {
            xa = (int(x0) / 32) * 32 + 32;
            ya = (xa - x0) * tanAlpha + y0;
            dy = 32 * tanAlpha;
            dx = 32;
        }
        else if (ra > P2 || ra < P3) // Промінь випущено вліво
        {
            xa = (int(x0) / 32) * 32 - 1;
            ya = (xa - x0) * tanAlpha + y0;
            dy = -32 * tanAlpha;
            dx = -32;
        }
        else {
            iter = 16;
        }

        if (xa >= 511 || ya >= 511 || xa <= 0 || ya < 0) { iter = 16; }
        else if (level[int(ya) / 32][int(xa) / 32]) { iter = 16; }
        else { iter = 1; }

        while (iter < 16) {
            iter++;

            xa += dx;
            ya += dy;

            if (xa >= 511 || ya >= 511 || xa <= 0 || ya < 0) { break; }
            if (level[int(ya) / 32][int(xa) / 32]) { break; }
        }
        xv = xa; yv = ya;

        // Перетин з горизонтальними лініями
        tanAlpha = tan(P2 - ra);
        if (ra < PI) // Промінь випущено вниз
        {
            ya = (int(y0) / 32) * 32 + 32;
            xa = (ya - y0) * tanAlpha + x0;
            dx = 32 * tanAlpha;
            dy = 32;
        }
        else if (ra > PI) // Промінь випущено вверх
        {
            ya = (int(y0) / 32) * 32 - 1;
            xa = (ya - y0) * tanAlpha + x0;
            dx = -32 * tanAlpha;
            dy = -32;
        }
        else {
            iter = 16;
        }

        if (xa >= 511 || ya >= 511 || xa <= 0 || ya < 0) { iter = 16; }
        else if (level[int(ya) / 32][int(xa) / 32]) { iter = 16; }
        else { iter = 1; }

        while (iter < 16) {
            iter++;

            xa += dx;
            ya += dy;

            if (xa >= 511 || ya >= 511 || xa <= 0 || ya < 0) { break; }
            if (level[int(ya) / 32][int(xa) / 32]) { break; }
        }
        xh = xa; yh = ya;

        if (dist_to_player(xh, yh) > dist_to_player(xv, yv))
        {
            end_cords[c/2] = Vector2f(xv, yv);
        }
        else {
            end_cords[c / 2] = Vector2f(xh, yh);
        }       
    }
}

float Player::dist_to_player(float x, float y)
{
    return sqrt((cords.x - x)*(cords.x - x) + (cords.y-y)*(cords.y-y));
}


// Базові функції 

void start_game() {

    VertexArray plates(Quads, 1024);

    // Завантажуємо схему рівня
    int** level = load_map_plan();
    print_matrix(level, 16);

    // Створюємо гравця
    Player player(256, 256, 1);
    CircleShape player_body(8);
    player_body.setFillColor(Color(0,255,123));
    VertexArray pl_dir(Lines, 2);
    pl_dir[0].position = player.cords;
    pl_dir[1].position = Vector2f(player.cords.x + 25*cos(player.dir), player.cords.x + 25 * sin(player.dir));

    // Масив променів
    VertexArray rays(Lines, WIN_WIDTH * 2);

    // Масив вершин зображення
    VertexArray projection(Lines, WIN_WIDTH * 2);

    ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Створюємо вікно
    RenderWindow window(VideoMode(842, 512), "RayCastingGameEngine", Style::Default, settings);
    window.setFramerateLimit(60);

    // Головний цикл програми
    while (window.isOpen())
    {
        // Обробка подій
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // управління
        if  (Keyboard::isKeyPressed(Keyboard::W)) { player.move_player(1); }
        else if (Keyboard::isKeyPressed(Keyboard::S)) { player.move_player(-1); }

        if (Keyboard::isKeyPressed(Keyboard::A)) { player.rotate_player(-1); }
        else if (Keyboard::isKeyPressed(Keyboard::D)) { player.rotate_player(1); }

        if (Keyboard::isKeyPressed(Keyboard::P)) { player.get_info(); }

        draw_minimap(level, plates);
        player_body.setPosition(Vector2f(player.cords.x - 8, player.cords.y - 8));
        pl_dir[0].position = player.cords;
        pl_dir[1].position = Vector2f(player.cords.x + 15 * cos(player.dir), player.cords.y + 15 * sin(player.dir));

        init_rays(rays, player);

        // Оновлюємо зображення
        window.clear();
        window.draw(projection);
        window.draw(plates);
        window.draw(player_body);
        window.draw(rays);
        player.cast_rays(level, window); 
        window.draw(pl_dir);
        window.display();
    }
}

int** load_map_plan()
{
    ifstream file;
    string n_str; // Розмір рівня рядком
    string cell_str;
    int cell;  
    int** level;
    int level_size; // Розмір рівня  

    file.open("../data/levels/level.csv");

    if (!file.is_open()) {
        cout << "Failed to load game map! File opening error.\n";
        return nullptr;
    }

    getline(file, n_str, '\n');
    level_size = stoi(n_str);

    level = new int* [level_size];

    for (int i = 0; i < level_size; i++)
    {
        level[i] = new int[level_size];

        for (int j = 0; j < level_size - 1; j++)
        {
            getline(file, cell_str, ',');
            cell = stoi(cell_str);
            level[i][j] = cell;
        }
        getline(file, cell_str, '\n');
        cell = stoi(cell_str);
        level[i][level_size - 1] = cell;
    }
    file.close();

    return level;
}

void draw_minimap(int** map, VertexArray & plates)
{

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            if (map[i][j])
            {
                plates[(i * 16 + j) * 4 + 0].color = Color(255, 255, 255);
                plates[(i * 16 + j) * 4 + 1].color = Color(255, 255, 255);
                plates[(i * 16 + j) * 4 + 2].color = Color(255, 255, 255);
                plates[(i * 16 + j) * 4 + 3].color = Color(255, 255, 255);
            }
            else {
                plates[(i * 16 + j) * 4 + 0].color = Color(90, 90, 90);
                plates[(i * 16 + j) * 4 + 1].color = Color(90, 90, 90);
                plates[(i * 16 + j) * 4 + 2].color = Color(90, 90, 90);
                plates[(i * 16 + j) * 4 + 3].color = Color(90, 90, 90);
            }
            plates[(i * 16 + j) * 4 + 0].position = Vector2f((j)     * 32,   (i)     * 32);
            plates[(i * 16 + j) * 4 + 1].position = Vector2f((j + 1) * 32-1, (i)     * 32);
            plates[(i * 16 + j) * 4 + 2].position = Vector2f((j + 1) * 32-1, (i + 1) * 32-1);
            plates[(i * 16 + j) * 4 + 3].position = Vector2f((j)     * 32,   (i + 1) * 32-1);
        }
    }
}

void init_rays(VertexArray & rays, Player & pl) {

    for (int i = 0; i < WIN_WIDTH*2; i+=2)
    {
        rays[i].position = pl.cords;
        rays[i].color = Color(255,(1/WIN_WIDTH)*255,255);
        rays[i + 1].position = pl.end_cords[i / 2];
        rays[i + 1].color = Color(255,255,255);
    }
}

double normalize_angle(double a)
{
    if (a > PI*2)
        a -= PI * 2;
    else if (a < 0)
        a += PI * 2;
    return a;
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

template<typename T>
void print_vector(T* vct, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << vct[i] << "\t";
    }
    cout << endl;
}

