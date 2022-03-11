#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#define IMG_PATH "src/libfifteen/img/"
#define FONT_NAME "Roboto-Regular.ttf"
#define GAME_WIDTH 600
#define GAME_HEIGHT 400
#define GAME_NAME "Fifteen Game"

class Game {
    const int width = GAME_WIDTH;
    const int height = GAME_HEIGHT;
    const char* windowName = GAME_NAME;

    int* numbers = new int[16];

    sf::RenderWindow window;

    int boardSize = 2;
    int cellSize = (width < height ? width : height) / 4 - boardSize;

    int countSteps = 0;
    int bestSteps = 0;

    void getCoord(int i, int& outX, int& outY);

    void draw();

    void drawStats();

    void swapNumbers(int idx1, int idx2);

    int getCellIdx(int x, int y);

    typedef enum Movement {
        NO_MOVE = -1,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_UP,
        MOVE_DOWN
    } Movement;

    Movement getMovement(int idx);

    void setBestResult();
    void getBestResult();

public:
    Game();

    void run();
};

#endif
