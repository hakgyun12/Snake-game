#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

using namespace std;

class Position
{
    int x, y;
    Position(int col, int row);
    Position();
};

class snakeGame
{
private:
    int score, speed, width, height;
    char direction, snakeBody, wall, gate, growthItem, poisonItem;
    bool eatGrowthItem, eatPoisonItem;
    Position growthItem, poisonItem;
    vector<Position> snake;

    void createMap();
    void showWindow();
    void showSnake();
    void snakePosition();
    bool getGrowthItem();
    bool getPoisonItem();
    bool collision();
    void randomGate();
    void printScore();
    void snakeMove();

public:
    snakeGame();
    ~snakeGame();
    void playGame();
};