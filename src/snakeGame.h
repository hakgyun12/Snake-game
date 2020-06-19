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
    char direction, partchar, edgechar, growthItemchar, poisonItemchar, speedItemchar; // partchar is the character representing the snake's body
    // edgechar is the character representing the edge of the game window
    // growthItemchar is the character representing the growthItem
    // del stands for delay
    int scoreGrowthItem, scorePoisonItem, scoreGate, scoreSpeedItem, currentLength;
    int growthItemTimer, poisonItemTimer, speedItemTimer, gateTimer; // maxLength 필요 없어 보임
    int requiredLength, requiredGrowthItem, requiredPoisonItem, requiredGate, requiredSpeedItem;
    bool bEatsGrowth, bEatsPoison, bAtGate_1, bAtGate_2;
    CharPosition growthItem, poisonItem, speedItem; // need to clarify this combination
    std::vector<CharPosition> snake;                // represent the snake's body
    std::vector<CharPosition> wall;
    CharPosition gate_1, gate_2;
    bool isClear;

    void InitGameWindow(int level);
    void DrawWindow(int level);
    void DrawSnake();
    void PrintScore();
    bool NextStage();
    //bool CheckNextStage();
    void PositionGrowth();
    void PositionPoison();
    void PositionSpeed();
    void PositionGate();
    bool FatalCollision();
    void MoveSnake();
    bool GetsGrowth();
    bool GetsPoison();
    void GetsSpeed();
    bool GetsGate();
    void growthItemTime();
    void poisonItemTime();
    void speedItemTime();
    void gateTime();
    char getWarpDirection(char d, CharPosition gate);

public:
    snakeGame(int level);
    ~snakeGame(); // destructor for cleanup and memory deallocation
    void PlayGame();
    bool getClear();
};

#endif