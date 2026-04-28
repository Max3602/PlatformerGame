#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

// Структура для монетки
struct Coin {
    RectangleShape shape;
    bool collected;
};

// Структура для ДВЕРИ
struct Door {
    RectangleShape shape;
    bool isActive;      // активна ли дверь (появляется после сбора всех монет)
    Vector2f position;

    Door() {
        shape.setSize(Vector2f(40, 60));
        shape.setFillColor(Color(0, 200, 0));  // зелёная дверь (активная)
        shape.setOutlineColor(Color::White);
        shape.setOutlineThickness(2);
        isActive = false;
    }

    void setPosition(float x, float y) {
        position = Vector2f(x, y);
        shape.setPosition(Vector2f(x, y));
    }

    void update(bool allCoinsCollected) {
        isActive = allCoinsCollected;
        if (isActive) {
            shape.setFillColor(Color(0, 200, 0));  // зелёная — вход открыт
        }
        else {
            shape.setFillColor(Color(100, 100, 100));  // серая — закрыто
        }
    }

    void draw(RenderWindow& window) {
        if (isActive) {
            window.draw(shape);
        }
    }

    FloatRect getBounds() {
        return FloatRect(position, Vector2f(40, 60));
    }
};

// Структура для уровня
struct Level {
    std::vector<RectangleShape> platforms;
    std::vector<Coin> coins;
    Door door;
    int coinsToNextLevel;
    Vector2f playerStart;
};

int main()
{
    // --- НАСТРОЙКИ КАМЕРЫ ---
    const float WORLD_WIDTH = 1920.0f;
    const float WORLD_HEIGHT = 1200.0f;

    RenderWindow window(VideoMode({ 1920, 1080 }), "Platformer Game");
    window.setFramerateLimit(60);

    // --- ВИД (КАМЕРА) ---
    View camera(FloatRect(Vector2f(0, 0), Vector2f(1920, 1080)));

    // --- Шрифт ---
    Font font;
    font.openFromFile("arial.ttf");

    // --- Тексты ---
    Text scoreText(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(Vector2f(20, 20));

    Text restartText(font);
    restartText.setCharacterSize(24);
    restartText.setFillColor(Color::White);
    restartText.setPosition(Vector2f(20, 70));
    restartText.setString("Press R to restart");

    Text levelText(font);
    levelText.setCharacterSize(36);
    levelText.setFillColor(Color::White);
    levelText.setPosition(Vector2f(900, 20));

    Text doorText(font);
    doorText.setCharacterSize(28);
    doorText.setFillColor(Color::Yellow);
    doorText.setPosition(Vector2f(0, 0));  // позиция будет обновляться

    Text winText(font);
    winText.setCharacterSize(60);
    winText.setFillColor(Color::Green);
    winText.setPosition(Vector2f(700, 500));
    winText.setString("YOU WIN! Press R to play again");

    bool gameWon = false;
    int currentLevel = 1;
    int totalScore = 0;

    // --- ИГРОК (КУБИК) ---
    RectangleShape player(Vector2f(100, 100));
    player.setFillColor(Color(80, 80, 80));
    Vector2f startPosition(100, 500);
    player.setPosition(startPosition);

    // --- Переменные уровней ---
    std::vector<RectangleShape> platforms;
    std::vector<Coin> coins;
    Door door;
    int score = 0;

    // --- Создание монетки ---
    auto createCoin = [](float x, float y) -> Coin {
        Coin c;
        c.shape = RectangleShape(Vector2f(20, 20));
        c.shape.setFillColor(Color(255, 215, 0));
        c.shape.setPosition(Vector2f(x, y));
        c.collected = false;
        return c;
        };

    // ----- УРОВЕНЬ 1 -----
    Level level1;
    level1.playerStart = Vector2f(100, 500);
    level1.coinsToNextLevel = 3;
    level1.door.setPosition(1700, 720);  // дверь справа

    RectangleShape l1_p1(Vector2f(200, 30));
    l1_p1.setFillColor(Color(150, 100, 50));
    l1_p1.setPosition(Vector2f(100, 800));
    level1.platforms.push_back(l1_p1);

    RectangleShape l1_p2(Vector2f(150, 30));
    l1_p2.setFillColor(Color(150, 100, 50));
    l1_p2.setPosition(Vector2f(500, 700));
    level1.platforms.push_back(l1_p2);

    RectangleShape l1_p3(Vector2f(300, 30));
    l1_p3.setFillColor(Color(150, 100, 50));
    l1_p3.setPosition(Vector2f(1000, 600));
    level1.platforms.push_back(l1_p3);

    level1.coins.push_back(createCoin(150, 750));
    level1.coins.push_back(createCoin(550, 650));
    level1.coins.push_back(createCoin(1050, 550));

    // ----- УРОВЕНЬ 2 -----
    Level level2;
    level2.playerStart = Vector2f(100, 400);
    level2.coinsToNextLevel = 4;
    level2.door.setPosition(1700, 520);  // дверь справа

    RectangleShape l2_p1(Vector2f(180, 30));
    l2_p1.setFillColor(Color(150, 100, 50));
    l2_p1.setPosition(Vector2f(100, 700));
    level2.platforms.push_back(l2_p1);

    RectangleShape l2_p2(Vector2f(150, 30));
    l2_p2.setFillColor(Color(150, 100, 50));
    l2_p2.setPosition(Vector2f(450, 600));
    level2.platforms.push_back(l2_p2);

    RectangleShape l2_p3(Vector2f(200, 30));
    l2_p3.setFillColor(Color(150, 100, 50));
    l2_p3.setPosition(Vector2f(800, 500));
    level2.platforms.push_back(l2_p3);

    RectangleShape l2_p4(Vector2f(250, 30));
    l2_p4.setFillColor(Color(150, 100, 50));
    l2_p4.setPosition(Vector2f(1200, 600));
    level2.platforms.push_back(l2_p4);

    level2.coins.push_back(createCoin(140, 650));
    level2.coins.push_back(createCoin(500, 550));
    level2.coins.push_back(createCoin(850, 450));
    level2.coins.push_back(createCoin(1250, 550));

    // ----- УРОВЕНЬ 3 -----
    Level level3;
    level3.playerStart = Vector2f(100, 300);
    level3.coinsToNextLevel = 5;
    level3.door.setPosition(1700, 350);  // дверь справа

    RectangleShape l3_p1(Vector2f(150, 30));
    l3_p1.setFillColor(Color(150, 100, 50));
    l3_p1.setPosition(Vector2f(100, 650));
    level3.platforms.push_back(l3_p1);

    RectangleShape l3_p2(Vector2f(150, 30));
    l3_p2.setFillColor(Color(150, 100, 50));
    l3_p2.setPosition(Vector2f(400, 550));
    level3.platforms.push_back(l3_p2);

    RectangleShape l3_p3(Vector2f(150, 30));
    l3_p3.setFillColor(Color(150, 100, 50));
    l3_p3.setPosition(Vector2f(700, 450));
    level3.platforms.push_back(l3_p3);

    RectangleShape l3_p4(Vector2f(150, 30));
    l3_p4.setFillColor(Color(150, 100, 50));
    l3_p4.setPosition(Vector2f(1000, 550));
    level3.platforms.push_back(l3_p4);

    RectangleShape l3_p5(Vector2f(200, 30));
    l3_p5.setFillColor(Color(150, 100, 50));
    l3_p5.setPosition(Vector2f(1300, 450));
    level3.platforms.push_back(l3_p5);

    level3.coins.push_back(createCoin(140, 600));
    level3.coins.push_back(createCoin(440, 500));
    level3.coins.push_back(createCoin(740, 400));
    level3.coins.push_back(createCoin(1040, 500));
    level3.coins.push_back(createCoin(1350, 400));

    std::vector<Level> levels = { level1, level2, level3 };

    // --- Загрузка уровня ---
    auto loadLevel = [&](int levelNum) {
        if (levelNum <= (int)levels.size()) {
            Level& lvl = levels[levelNum - 1];
            platforms = lvl.platforms;
            coins = lvl.coins;
            door = lvl.door;
            player.setPosition(lvl.playerStart);
            startPosition = lvl.playerStart;
            score = 0;
        }
        };

    loadLevel(1);

    // --- Физика ---
    Vector2f velocity(0, 0);
    float gravity = 800.0f;
    bool isOnGround = false;

    Clock deltaClock;

    // --- Флаг для предотвращения многократного перехода ---
    bool doorPressed = false;

    while (window.isOpen())
    {
        float deltaTime = deltaClock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        if (currentLevel > (int)levels.size()) {
            gameWon = true;
        }

        if (!gameWon) {
            // --- Управление ---
            if (Keyboard::isKeyPressed(Keyboard::Key::Left))
                velocity.x = -200.0f;
            else if (Keyboard::isKeyPressed(Keyboard::Key::Right))
                velocity.x = 200.0f;
            else
                velocity.x = 0.0f;

            if (Keyboard::isKeyPressed(Keyboard::Key::Up) && isOnGround)
            {
                velocity.y = -800.0f;
                isOnGround = false;
            }

            // --- Рестарт ---
            if (Keyboard::isKeyPressed(Keyboard::Key::R))
            {
                player.setPosition(startPosition);
                velocity = Vector2f(0, 0);
                score = 0;
                if (currentLevel <= (int)levels.size()) {
                    Level& lvl = levels[currentLevel - 1];
                    coins = lvl.coins;
                }
                doorPressed = false;
            }

            // --- Гравитация ---
            velocity.y += gravity * deltaTime;
            player.move(velocity * deltaTime);

            // --- Проверка столкновения с платформами ---
            isOnGround = false;
            FloatRect playerBounds(player.getPosition(), Vector2f(100, 100));

            for (auto& plat : platforms)
            {
                FloatRect platBounds(plat.getPosition(), plat.getSize());

                if (playerBounds.findIntersection(platBounds) && velocity.y > 0 &&
                    player.getPosition().y + 100 <= plat.getPosition().y + 30)
                {
                    player.setPosition(Vector2f(player.getPosition().x, plat.getPosition().y - 100));
                    velocity.y = 0;
                    isOnGround = true;
                    break;
                }
            }

            // Ограничение по левому краю
            if (player.getPosition().x < 20) {
                player.setPosition(Vector2f(20, player.getPosition().y));
            }

            // --- Падение в пропасть ---
            if (player.getPosition().y > 1200) {
                player.setPosition(startPosition);
                velocity = Vector2f(0, 0);
            }

            // --- Сбор монеток ---
            for (auto& coin : coins)
            {
                if (!coin.collected)
                {
                    FloatRect playerBounds(player.getPosition(), Vector2f(100, 100));
                    FloatRect coinBounds(coin.shape.getPosition(), Vector2f(20, 20));

                    if (playerBounds.findIntersection(coinBounds))
                    {
                        coin.collected = true;
                        score += 10;
                        totalScore += 10;
                    }
                }
            }

            // --- ПОДСЧЁТ СОБРАННЫХ МОНЕТ ---
            int collectedCount = 0;
            for (auto& coin : coins) {
                if (coin.collected) collectedCount++;
            }

            // --- ОБНОВЛЕНИЕ ДВЕРИ (активна, если собраны все монеты уровня) ---
            Level& currentLvl = levels[currentLevel - 1];
            bool allCoinsCollected = (collectedCount >= currentLvl.coinsToNextLevel);
            door.update(allCoinsCollected);

            // --- ПЕРЕХОД К ДВЕРИ ПО КЛАВИШЕ E ---
            if (door.isActive) {
                FloatRect playerBounds(player.getPosition(), Vector2f(100, 100));
                FloatRect doorBounds = door.getBounds();

                if (playerBounds.findIntersection(doorBounds)) {
                    // Показываем подсказку
                    doorText.setString("Press E to enter the door");
                    float textX = door.position.x - 50;
                    float textY = door.position.y - 40;
                    doorText.setPosition(Vector2f(textX, textY));

                    // Переход по клавише E
                    if (Keyboard::isKeyPressed(Keyboard::Key::E) && !doorPressed) {
                        doorPressed = true;
                        currentLevel++;
                        if (currentLevel <= (int)levels.size()) {
                            loadLevel(currentLevel);
                        }
                    }
                    else if (!Keyboard::isKeyPressed(Keyboard::Key::E)) {
                        doorPressed = false;
                    }
                }
                else {
                    doorText.setString("");
                }
            }
            else {
                doorText.setString("");
            }
        }

        // --- Рестарт всей игры ---
        if (gameWon && Keyboard::isKeyPressed(Keyboard::Key::R)) {
            gameWon = false;
            currentLevel = 1;
            totalScore = 0;
            loadLevel(1);
            doorPressed = false;
        }

        // --- Обновление текста ---
        if (!gameWon) {
            scoreText.setString("Score: " + std::to_string(score) + "  Total: " + std::to_string(totalScore));
        }
        else {
            scoreText.setString("Final Score: " + std::to_string(totalScore));
        }
        levelText.setString("Level: " + std::to_string(currentLevel) + "/" + std::to_string(levels.size()));

        // --- ОБНОВЛЕНИЕ КАМЕРЫ ---
        float cameraX = player.getPosition().x + 50 - 960;
        float cameraY = player.getPosition().y + 50 - 540;

        if (cameraX < 0) cameraX = 0;
        if (cameraY < 0) cameraY = 0;
        if (cameraX > WORLD_WIDTH - 1920) cameraX = WORLD_WIDTH - 1920;
        if (cameraY > WORLD_HEIGHT - 1080) cameraY = WORLD_HEIGHT - 1080;

        camera.setCenter(Vector2f(cameraX + 960, cameraY + 540));

        // --- Отрисовка ---
        window.setView(camera);

        window.clear(Color(60, 60, 60));

        for (auto& plat : platforms)
            window.draw(plat);

        for (auto& coin : coins)
            if (!coin.collected)
                window.draw(coin.shape);

        door.draw(window);

        window.draw(player);

        // --- UI текст (без камеры) ---
        window.setView(window.getDefaultView());
        window.draw(scoreText);
        window.draw(restartText);
        window.draw(levelText);
        window.draw(doorText);

        if (gameWon) {
            window.draw(winText);
        }

        window.display();
    }

    return 0;
}