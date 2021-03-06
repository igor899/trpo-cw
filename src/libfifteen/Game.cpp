#include <libfifteen/Game.hpp>

Game::Game()
{
    srand(time(0));

    for (int i = 0; i < 16; i++)
        numbers[i] = i + 1;
    numbers[15] = 0;

    newGame();
}

void Game::newGame()
{
    countSteps = 0;
    getBestResult();
    printWin = false;
}

void Game::run()
{
    window = new sf::RenderWindow(
            sf::VideoMode(width, height), windowName, sf::Style::Close);

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed
                || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                window->close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                gameStarted = true;
                randomize();
                newGame();
            }

            if (gameStarted) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    int idx = getIdxOfZero();
                    if (getMovement(idx + 4) == MOVE_UP) {
                        swapNumbers(idx, idx + 4);
                        ++countSteps;
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    int idx = getIdxOfZero();
                    if (getMovement(idx - 4) == MOVE_DOWN) {
                        swapNumbers(idx, idx - 4);
                        ++countSteps;
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    int idx = getIdxOfZero();
                    if (getMovement(idx + 1) == MOVE_LEFT) {
                        swapNumbers(idx, idx + 1);
                        ++countSteps;
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    int idx = getIdxOfZero();
                    if (getMovement(idx - 1) == MOVE_RIGHT) {
                        swapNumbers(idx, idx - 1);
                        ++countSteps;
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && gameStarted) {
                int cellIdx = getCellIdx(
                        sf::Mouse::getPosition(*window).x,
                        sf::Mouse::getPosition(*window).y);
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
        window->clear();
        draw();
        checkWin();
        window->display();
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
    fp = fopen(FILENAME_STAT, "wb");
    if (!fp) {
        std::cerr << "Error! ???? ???????? ???????????????? ?? ????????!\n";
        return;
    }
    fwrite(&countSteps, sizeof(int), 1, fp);
    fclose(fp);

    getBestResult();
}

void Game::getBestResult()
{
    FILE* fp;
    fp = fopen(FILENAME_STAT, "rb");
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
            L"R : ???????????????????? ?????????? \n"
            "Q : ?????????? \n",
            font,
            14);
    text.setPosition(sf::Vector2f(xPosition, yPosition));
    text.setFillColor(sf::Color::Cyan);

    auto statText
            = L"???????????????????? ??????????: \n"
              "???????????? ??????????????????: \n";
    sf::Text stat(statText, font, 14);
    stat.setPosition(sf::Vector2f(xPosition, yPosition + (int)(height * 0.85)));
    stat.setFillColor(sf::Color::Cyan);

    auto statResultText
            = std::to_string(countSteps) + "\n" + std::to_string(bestSteps);

    sf::Text statResult(statResultText, font, 14);
    statResult.setPosition(sf::Vector2f(
            width - 15 * boardSize, yPosition + (int)(height * 0.85)));
    statResult.setFillColor(sf::Color::Cyan);

    window->draw(text);
    window->draw(stat);
    window->draw(statResult);

    if (printWin && countSteps && gameStarted) {
        sf::Text winText(L"???? ????????????????!", font, 14);
        winText.setPosition(
                sf::Vector2f(xPosition, yPosition + (int)(height * 0.6)));
        winText.setFillColor(sf::Color::Green);
        window->draw(winText);
    }

    if (countSteps == 0 && gameStarted == false) {
        sf::Text winText(L"?????????????? R ?????????? ????????????", font, 14);
        winText.setPosition(
                sf::Vector2f(xPosition, yPosition + (int)(height * 0.7)));
        winText.setFillColor(sf::Color::Yellow);
        window->draw(winText);
    }
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
        window->draw(sprite);
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

int Game::getIdxOfZero()
{
    for (int i = 0; i < 16; i++)
        if (numbers[i] == 0)
            return i;
    return -1;
}

void Game::randomize()
{
    int randIdx;
    for (int i = 0; i < 16; ++i) {
        randIdx = rand() % 16;
        swapNumbers(i, randIdx);
    }
}

bool Game::isWin()
{
    if (numbers[0] == 1 && numbers[15] == 0) {
        for (int i = 0; i < 14; i++)
            if (numbers[i + 1] < numbers[i])
                return false;
        return true;
    }
    return false;
}

void Game::checkWin()
{
    printWin = false;
    if (isWin() && gameStarted) {
        if (countSteps && (bestSteps == 0 || bestSteps > countSteps))
            setBestResult();
        printWin = true;
    }
}
