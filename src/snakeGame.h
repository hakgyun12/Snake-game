#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Position
{
    int x, y;
    Position(int col, int row);
    Position();
};

class snakeGame
{
private:
    int score, speed, width, height;
    int countGrowthItem, countPoisonItem;
    int scoreGrowthItem, scorePoisonItem, snakeLength, scoreGate;
    char ection, snakeBody, gate, growthItemChar, poisonItemChar;
    char upRightEdge, upLeftEdge, downRightEdge, downLeftEdge, rowLine, colLine;
    char direction;
    bool eatGrowthItem, eatPoisonItem;
    Position growthItem, poisonItem;
    vector<Position> snake;
    vector<Position> item;

    void initGame();
    void drawWindow();
    void drawSnake();
    void printScore();
    void positionItem();
    void snakePosition();
    bool getGrowthItem();
    bool getPoisonItem();
    bool collision();
    void randomGate();
    void snakeMove();
    void createMap();

public:
    snakeGame();
    ~snakeGame();
    void playGame();
};