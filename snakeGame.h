#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

#ifndef SNAKEGAME_H
#define SNAKEGAME_H
// Unreal standards

struct CharPosition
{
    int x, y;
    CharPosition(int col, int row);
    CharPosition();
};

class snakeGame
{
private:
    int speed, itemChange, maxwidth, maxheight, gateChange;
    char direction, partchar, edgechar, growthItemchar, poisonItemchar;
    // partchar is the character representing the snake's body
    // edgechar is the character representing the edge of the game window
    // growthItemchar is the character representing the growthItem
    // del stands for delay
    int scoreGrowthItem, scorePoisonItem, scoreGate, currentLength;
    int growthItemTimer, poisonItemTimer, gateTimer; // maxLength 필요 없어 보임
    int requiredLength, requiredGrowthItem, requiredPoisonItem, requiredGate;
    bool bEatsGrowth, bEatsPoison, bAtGate_1, bAtGate_2;
    CharPosition growthItem, poisonItem; // need to clarify this combination
    std::vector<CharPosition> snake;     // represent the snake's body
    std::vector<CharPosition> wall;
    CharPosition gate_1, gate_2;

    void InitGameWindow();
    void DrawWindow();
    void DrawSnake();
    void PrintScore();
    void PositionGrowth();
    void PositionPoison();
    void PositionGate();
    bool FatalCollision();
    void MoveSnake();
    bool GetsGrowth();
    bool GetsPoison();
    bool GetsGate();
    void growthItemTime();
    void poisonItemTime();
    void gateTime();

public:
    snakeGame();
    ~snakeGame();
    void PlayGame();
};

#endif