#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int blockSize = 20;
const int width = 30;
const int height = 20;
int speed = 10;

struct SnakeBlock {
    int x, y;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(width * blockSize, height * blockSize), "Snake Game");
    window.setFramerateLimit(60);

    sf::RectangleShape snakeShape(sf::Vector2f(blockSize - 1, blockSize - 1));
    snakeShape.setFillColor(sf::Color::Green);

    sf::RectangleShape foodShape(sf::Vector2f(blockSize - 1, blockSize - 1));
    foodShape.setFillColor(sf::Color::Red);

    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text gameOverText("Game Over!", font, 40);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition(width * blockSize / 3, height * blockSize / 2);

    sf::SoundBuffer eatBuffer, deathBuffer;
    eatBuffer.loadFromFile("eat.wav");
    deathBuffer.loadFromFile("death.wav");
    sf::Sound eatSound(eatBuffer);
    sf::Sound deathSound(deathBuffer);

    std::vector<SnakeBlock> snake = {{10, 10}};
    int dx = 1, dy = 0;

    SnakeBlock food = {rand() % width, rand() % height};

    sf::Clock clock;
    float timer = 0;
    float delay = 0.1;

    while (window.isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        // Event Handling
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
        }

        // Keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dy == 0) {
            dx = 0; dy = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dy == 0) {
            dx = 0; dy = 1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dx == 0) {
            dx = -1; dy = 0;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dx == 0) {
            dx = 1; dy = 0;
        }

        // Game logic
        if (timer > delay) {
            timer = 0;

            SnakeBlock newHead = snake[0];
            newHead.x += dx;
            newHead.y += dy;

            // Wall collision
            if (newHead.x < 0 || newHead.x >= width || newHead.y < 0 || newHead.y >= height) {
                deathSound.play();
                window.draw(gameOverText);
                window.display();
                sf::sleep(sf::seconds(2));
                window.close();
                break;
            }

            // Self-collision
            for (auto& b : snake) {
                if (b.x == newHead.x && b.y == newHead.y) {
                    deathSound.play();
                    window.draw(gameOverText);
                    window.display();
                    sf::sleep(sf::seconds(2));
                    window.close();
                    break;
                }
            }

            snake.insert(snake.begin(), newHead);

            if (newHead.x == food.x && newHead.y == food.y) {
                eatSound.play();
                food = {rand() % width, rand() % height};
                if (delay > 0.03f) delay -= 0.005f; // Increase speed
            } else {
                snake.pop_back();
            }
        }

        // Drawing
        window.clear();

        for (auto& b : snake) {
            snakeShape.setPosition(b.x * blockSize, b.y * blockSize);
            window.draw(snakeShape);
        }

        foodShape.setPosition(food.x * blockSize, food.y * blockSize);
        window.draw(foodShape);

        window.display();
    }

    return 0;
}
