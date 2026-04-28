#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;

int main()
{
    RenderWindow window(VideoMode({ 1920, 1080 }), "Platformer Game");
    window.setFramerateLimit(60);

    // --- Шрифт ---
    Font font;
    font.openFromFile("arial.ttf");  // если нет файла — текст не появится, но игра не упадёт

    // --- Текст счёта (СОЗДАЁМ СРАЗУ СО ШРИФТОМ) ---
    Text scoreText(font);  // ← вот так правильно для SFML 3!
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(Vector2f(20, 20));

    // --- Текст рестарта (СОЗДАЁМ СРАЗУ СО ШРИФТОМ) ---
    Text restartText(font);  // ← вот так правильно для SFML 3!
    restartText.setCharacterSize(24);
    restartText.setFillColor(Color::White);
    restartText.setPosition(Vector2f(20, 70));
    restartText.setString("Press R to restart");

    int score = 0;

    // --- Игрок ---
    RectangleShape player(Vector2f(100, 100));
    player.setFillColor(Color::Green);
    Vector2f startPosition(100, 500);
    player.setPosition(startPosition);

    // --- Платформы ---
    std::vector<RectangleShape> platforms;

    RectangleShape p1(Vector2f(200, 30));
    p1.setFillColor(Color::Yellow);
    p1.setPosition(Vector2f(100, 800));
    platforms.push_back(p1);

    RectangleShape p2(Vector2f(150, 30));
    p2.setFillColor(Color::Yellow);
    p2.setPosition(Vector2f(500, 700));
    platforms.push_back(p2);

    RectangleShape p3(Vector2f(300, 30));
    p3.setFillColor(Color::Yellow);
    p3.setPosition(Vector2f(1000, 600));
    platforms.push_back(p3);

    // --- Монетки ---
    struct Coin {
        RectangleShape shape;
        bool collected;
    };

    std::vector<Coin> coins;

    auto createCoins = [&]() {
        coins.clear();

        Coin c1;
        c1.shape = RectangleShape(Vector2f(20, 20));
        c1.shape.setFillColor(Color::Yellow);
        c1.shape.setPosition(Vector2f(150, 750));
        c1.collected = false;
        coins.push_back(c1);

        Coin c2;
        c2.shape = RectangleShape(Vector2f(20, 20));
        c2.shape.setFillColor(Color::Yellow);
        c2.shape.setPosition(Vector2f(550, 650));
        c2.collected = false;
        coins.push_back(c2);

        Coin c3;
        c3.shape = RectangleShape(Vector2f(20, 20));
        c3.shape.setFillColor(Color::Yellow);
        c3.shape.setPosition(Vector2f(1050, 550));
        c3.collected = false;
        coins.push_back(c3);
        };

    createCoins();

    // --- Физика ---
    Vector2f velocity(0, 0);
    float gravity = 800.0f;
    bool isOnGround = false;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

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
            createCoins();
        }

        // --- Гравитация ---
        velocity.y += gravity * (1.0f / 60.0f);
        player.move(velocity * (1.0f / 60.0f));

        // --- Проверка столкновения с платформами ---
        isOnGround = false;

        for (auto& plat : platforms)
        {
            if (player.getPosition().y + 100 >= plat.getPosition().y &&
                player.getPosition().y + 100 <= plat.getPosition().y + 30 &&
                player.getPosition().x + 100 > plat.getPosition().x &&
                player.getPosition().x < plat.getPosition().x + plat.getSize().x &&
                velocity.y > 0)
            {
                player.setPosition(Vector2f(player.getPosition().x, plat.getPosition().y - 100));
                velocity.y = 0;
                isOnGround = true;
                break;
            }
        }

        // --- Сбор монеток ---
        for (auto& coin : coins)
        {
            if (!coin.collected &&
                player.getPosition().x + 100 > coin.shape.getPosition().x &&
                player.getPosition().x < coin.shape.getPosition().x + 20 &&
                player.getPosition().y + 100 > coin.shape.getPosition().y &&
                player.getPosition().y < coin.shape.getPosition().y + 20)
            {
                coin.collected = true;
                score += 10;
            }
        }

        // --- Обновление текста ---
        scoreText.setString("Score: " + std::to_string(score));

        // --- Отрисовка ---
        window.clear(Color::Magenta);

        for (auto& plat : platforms)
            window.draw(plat);

        for (auto& coin : coins)
            if (!coin.collected)
                window.draw(coin.shape);

        window.draw(player);
        window.draw(scoreText);
        window.draw(restartText);

        window.display();
    }

    return 0;
}