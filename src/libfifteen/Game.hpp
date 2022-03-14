#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include <ctime>
#include <iostream>
#include <random>
#include <string>

#include <libfifteen/constants.hpp>

class Game {
protected:
    const int width = GAME_WIDTH;
    const int height = GAME_HEIGHT;
    const char* windowName = GAME_NAME;

    bool printWin = false;
    bool gameStarted = false;

    int* numbers = new int[16];

    sf::RenderWindow* window = nullptr;

    int boardSize = 2;
    int cellSize = (width < height ? width : height) / 4 - boardSize;

    int countSteps = 0;
    int bestSteps = 0;

    void getCoord(int i, int& outX, int& outY);

    void draw();
    void drawStats();

    void newGame();

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

    void randomize();

    bool isWin();
    void checkWin();

    int getIdxOfZero();

    friend class GameTest;

public:
    Game();

    void run();
};

#endif
