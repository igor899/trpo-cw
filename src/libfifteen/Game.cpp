#include <libfifteen/Game.hpp>

Game::Game() : window(sf::VideoMode(width, height), windowName)
{
    for (int i = 0; i < 16; i++)
        numbers[i] = i + 1;
    numbers[15] = 0;
}

void Game::run()
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.display();
    }
}
