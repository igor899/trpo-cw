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
    std::string imgPath = IMG_PATH;

    int x, y;

    sf::Texture texture;
    sf::Texture number;
    texture.loadFromFile(imgPath + "tile_64x64.png");
    sf::Sprite spriteTexture(texture);

    sf::RenderTexture renderTexture;

    number.loadFromFile(imgPath + "1.png");

    for (int i = 0; i < 16; i++) {
        renderTexture.clear();

        renderTexture.create(cellSize, cellSize);

        renderTexture.draw(spriteTexture);

        if (numbers[i] != 0) {
            sf::Sprite spriteNumber(number);
            number.loadFromFile(imgPath + std::to_string(numbers[i]) + ".png");
            renderTexture.draw(spriteNumber);
        }

        getCoord(i, x, y);

        renderTexture.display();

        sf::Sprite sprite(renderTexture.getTexture());
        sprite.setPosition(x, y);
        sprite.setScale(cellSize * 1.0 / 64, cellSize * 1.0 / 64);
        window.draw(sprite);
    }
}

void Game::swapNumbers(int idx1, int idx2)
{
    int temp = numbers[idx1];
    numbers[idx1] = numbers[idx2];
    numbers[idx2] = temp;
}

int Game::getCellIdx(int x, int y)
{
    if (x < 0 || x > 4 * cellSize || y < 0 || y > 4 * cellSize)
        return -1;
    int xIdx = x / cellSize;
    int yIdx = y / cellSize;
    int resultIdx = yIdx * 4 + xIdx;
    if (resultIdx < 0 || resultIdx > 15)
        return -1;
    return resultIdx;
}
