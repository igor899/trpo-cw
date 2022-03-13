#ifndef GAMETEST_HPP
#define GAMETEST_HPP

#include <gtest/gtest.h>
#include <libfifteen/Game.hpp>

class GameTest : public ::testing::Test {
protected:
    int* get_numbers()
    {
        return game.numbers;
    };
    bool get_printWin()
    {
        return game.printWin;
    };
    bool get_gameStarted()
    {
        return game.gameStarted;
    };
    int get_countSteps()
    {
        return game.countSteps;
    };
    int get_bestSteps()
    {
        return game.bestSteps;
    };

    void newGame()
    {
        game.newGame();
    };
    void swapNumbers(int idx1, int idx2)
    {
        game.swapNumbers(idx1, idx2);
    };
    void setBestResult()
    {
        game.setBestResult();
    };
    void getBestResult()
    {
        game.getBestResult();
    };
    void randomize()
    {
        game.randomize();
    };
    void checkWin()
    {
        game.checkWin();
    };
    void getCoord(int i, int& outX, int& outY)
    {
        game.getCoord(i, outX, outY);
    };

    bool isWin()
    {
        return game.isWin();
    };
    int getCellIdx(int x, int y)
    {
        return game.getCellIdx(x, y);
    };
    int getIdxOfZero()
    {
        return game.getIdxOfZero();
    };
    Game::Movement getMovement(int idx)
    {
        return getMovement(idx);
    };

    void set_gameStarted_true()
    {
        game.gameStarted = true;
    };
    void set_countSteps(int x)
    {
        game.countSteps = x;
    };
    void set_bestSteps(int x)
    {
        game.bestSteps = x;
    };
    void set_printWin(bool x)
    {
        game.printWin = x;
    };

public:
    Game game;
};

TEST_F(GameTest, numbers)
{
    int* numbers = get_numbers();
    for (int i = 0; i < 15; i++)
        EXPECT_EQ(numbers[i], i + 1);
    EXPECT_EQ(numbers[15], 0);
}

TEST_F(GameTest, printWin)
{
    EXPECT_EQ(get_printWin(), false);
    checkWin();
    EXPECT_EQ(get_printWin(), false);

    set_gameStarted_true();
    checkWin();
    EXPECT_EQ(get_printWin(), true);
}

TEST_F(GameTest, gameStarted)
{
    EXPECT_EQ(get_gameStarted(), false);

    set_gameStarted_true();
    EXPECT_EQ(get_gameStarted(), true);
}

TEST_F(GameTest, countSteps)
{
    EXPECT_EQ(get_countSteps(), 0);

    set_countSteps(10);
    EXPECT_EQ(get_countSteps(), 10);
}

TEST_F(GameTest, bestSteps)
{
    set_bestSteps(10);
    EXPECT_EQ(get_bestSteps(), 10);
}

TEST_F(GameTest, getCoord)
{
    int x, y;
    getCoord(0, x, y);
    EXPECT_EQ(x, 0);
    EXPECT_EQ(y, 0);
}

TEST_F(GameTest, newGame)
{
    set_countSteps(10);
    setBestResult();
    set_printWin(true);

    newGame();

    EXPECT_EQ(get_countSteps(), 0);
    EXPECT_EQ(get_bestSteps(), 10);
    EXPECT_EQ(get_printWin(), false);
}

TEST_F(GameTest, swapNumbers)
{
    int* numbers = get_numbers();

    int x_prev = numbers[0];
    int y_prev = numbers[1];

    swapNumbers(0, 1);

    EXPECT_EQ(numbers[0], y_prev);
    EXPECT_EQ(numbers[1], x_prev);
}

TEST_F(GameTest, getCellIdx)
{
    int x_exp, x_real;
    int x, y;

    getCoord(1, x, y);
    x_real = getCellIdx(x, y);
    x_exp = 1;
    EXPECT_EQ(x_exp, x_real);

    getCoord(15, x, y);
    x_real = getCellIdx(x, y);
    x_exp = 15;
    EXPECT_EQ(x_exp, x_real);

    getCoord(7, x, y);
    x_real = getCellIdx(x, y);
    x_exp = 7;
    EXPECT_EQ(x_exp, x_real);
}

TEST_F(GameTest, getBestResult)
{
    set_countSteps(14);
    setBestResult();
    getBestResult();
    EXPECT_EQ(get_bestSteps(), 14);

    set_countSteps(141);
    setBestResult();
    getBestResult();
    EXPECT_EQ(get_bestSteps(), 141);
}

TEST_F(GameTest, setBestResult)
{
    set_countSteps(14);
    setBestResult();
    EXPECT_EQ(get_bestSteps(), 14);

    set_countSteps(141);
    setBestResult();
    EXPECT_EQ(get_bestSteps(), 141);
}

TEST_F(GameTest, randomize)
{
    int checkSum_exp = 0;
    int* numbers = get_numbers();
    for (int i = 0; i < 16; i++)
        checkSum_exp += numbers[i];

    randomize();

    int checkSum_real = 0;
    for (int i = 0; i < 16; i++)
        checkSum_real += numbers[i];

    EXPECT_EQ(checkSum_exp, checkSum_real);
}

TEST_F(GameTest, isWin)
{
    EXPECT_EQ(isWin(), true);
    randomize();
    EXPECT_EQ(isWin(), false);
}

TEST_F(GameTest, checkWin)
{
    EXPECT_EQ(get_printWin(), false);
    checkWin();
    EXPECT_EQ(get_printWin(), false);

    set_gameStarted_true();
    checkWin();
    EXPECT_EQ(get_printWin(), true);

    set_countSteps(11);
    set_bestSteps(0);
    checkWin();
    EXPECT_EQ(get_printWin(), true);
    EXPECT_EQ(get_bestSteps(), 11);
}

TEST_F(GameTest, getIdxOfZero)
{
    int* numbers = get_numbers();
    int idx = getIdxOfZero();
    EXPECT_EQ(idx, 15);

    randomize();
    idx = getIdxOfZero();
    EXPECT_EQ(numbers[idx], 0);
}

#endif // #ifndef GAMETEST_HPP
