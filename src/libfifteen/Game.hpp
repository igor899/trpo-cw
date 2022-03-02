#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include <iostream>

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

public:
    Game();

    void run();
};

#endif
