#include <libfifteen/Game.hpp>

Game::Game()
    : window(sf::VideoMode(width, height), windowName, sf::Style::Close)
{
    for (int i = 0; i < 16; i++)
        numbers[i] = i + 1;
    numbers[15] = 0;

    getBestResult();
}

void Game::run()
{
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonReleased) {
                int cellIdx = getCellIdx(
                        sf::Mouse::getPosition(window).x,
                        sf::Mouse::getPosition(window).y);
                Movement move = getMovement(cellIdx);
                switch (move) {
                case MOVE_LEFT:
                    swapNumbers(cellIdx, cellIdx - 1);
                    ++countSteps;
                    break;
                case MOVE_RIGHT:
                    swapNumbers(cellIdx, cellIdx + 1);
                    ++countSteps;
                    break;
                case MOVE_UP:
                    swapNumbers(cellIdx, cellIdx - 4);
                    ++countSteps;
                    break;
                case MOVE_DOWN:
                    swapNumbers(cellIdx, cellIdx + 4);
                    ++countSteps;
                    break;

                default:
                    break;
                }
            }
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

void Game::setBestResult()
{
    FILE* fp;
    fp = fopen("stat.bin", "wb");
    if (!fp) {
        std::cerr << "Error occured!\n";
        return;
    }
    fwrite(&countSteps, sizeof(int), 1, fp);
    fclose(fp);

    getBestResult();
}

void Game::getBestResult()
{
    FILE* fp;
    fp = fopen("stat.bin", "rb");
    if (!fp) {
        return;
    }
    fread(&bestSteps, sizeof(int), 1, fp);
    fclose(fp);
}

void Game::drawStats()
{
    std::string imgPath = IMG_PATH;

    int xPosition = height + 2 * boardSize;
    int yPosition = boardSize + 2 * boardSize;

    sf::Font font;
    font.loadFromFile(imgPath + FONT_NAME);
    sf::Text text(
            L"R : перемешать фишки \n"
            "S : сохранить игру \n"
            "Q : выйти \n",
            font,
            14);
    text.setPosition(sf::Vector2f(xPosition, yPosition));
    text.setFillColor(sf::Color::Cyan);

    auto statText
            = L"Количество шагов: \n"
              "Лучший результат: \n";
    sf::Text stat(statText, font, 14);
    stat.setPosition(sf::Vector2f(xPosition, yPosition + (int)(height * 0.85)));
    stat.setFillColor(sf::Color::Cyan);

    auto statResultText
            = std::to_string(countSteps) + "\n" + std::to_string(bestSteps);

    sf::Text statResult(statResultText, font, 14);
    statResult.setPosition(sf::Vector2f(
            width - 15 * boardSize, yPosition + (int)(height * 0.85)));
    statResult.setFillColor(sf::Color::Cyan);

    window.draw(text);
    window.draw(stat);
    window.draw(statResult);
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

    drawStats();
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

Game::Movement Game::getMovement(int idx)
{
    if (idx % 4 != 0 && numbers[idx - 1] == 0)
        return MOVE_LEFT;
    if (idx % 4 != 3 && numbers[idx + 1] == 0)
        return MOVE_RIGHT;
    if (idx / 4 != 0 && numbers[idx - 4] == 0)
        return MOVE_UP;
    if (idx / 4 != 3 && numbers[idx + 4] == 0)
        return MOVE_DOWN;
    return NO_MOVE;
}
