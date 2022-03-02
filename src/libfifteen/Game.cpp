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
        window.clear();
        draw();
        window.display();
    }
}

void Game::getCoord(int idx, int& outX, int& outY)
{
    int cellHeight, cellWidth;
    cellHeight = cellWidth = height / 4;
    outX = cellWidth * (idx % 4);
    outY = cellHeight * (idx / 4);
}

void Game::draw()
{
    int x, y;

    for (int i = 0; i < 16; i++) {
        auto shape = sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
        getCoord(i, x, y);
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::White);
        window.draw(shape);
    }
}
