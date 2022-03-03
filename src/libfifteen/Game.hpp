#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>

#define IMG_PATH "src/libfifteen/img/"

class Game {
    const int width = 600;
    const int height = 400;
    const char* windowName = "Fifteen Game";

    int* numbers = new int[16];

    sf::RenderWindow window;

    int boardSize = 2;
    int cellSize = (width < height ? width : height) / 4 - boardSize;

    void getCoord(int i, int& outX, int& outY);

    void draw();

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

public:
    Game();

    void run();
};

#endif
