#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

#ifndef SNAKEGAME_H
#define SNAKEGAME_H
// Unreal standards
using int32 = int;

struct CharPosition
{
    int32 x, y;
    CharPosition(int32 col, int32 row);
    CharPosition();
};

class snakeGame
{
private:
    int32 score, del, maxwidth, maxheight;
    char direction, partchar, edgechar, fruitchar;
    // partchar is the character representing the snake's body
    // edgechar is the character representing the edge of the game window
    // fruitchar is the character representing the fruit
    // del stands for delay
    bool bEatsFruit;
    CharPosition fruit;              // need to clarify this combination
    std::vector<CharPosition> snake; // represent the snake's body

    void InitGameWindow();
    void DrawWindow();
    void DrawSnake();
    void PrintScore();
    void PositionFruit();
    bool FatalCollision();
    void MoveSnake();
    bool GetsFruit();

public:
    snakeGame();
    ~snakeGame(); // destructor for cleanup and memory deallocation
    void PlayGame();
};

#endif