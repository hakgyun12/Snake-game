#include "snakeGame.h"
#include <unistd.h>

using namespace std;

Position::Position(int col, int row)
{
    x = col;
    y = row;
}

Position::Position()
{
    x = 0;
    y = 0;
}

snakeGame::snakeGame()
{
    snakeBody = 'x';
    upRightEdge = (char)187;
    upLeftEdge = (char)201;
    downRightEdge = (char)188;
    downLeftEdge = (char)200;
    rowLine = (char)205;
    colLine = (char)186;

    growthItemChar = '+';
    poisonItemChar = '-';
    growthItem.x = 0;
    growthItem.y = 0;
    poisonItem.x = 1;
    poisonItem.y = 1;
    score = 0;
    speed = 80000;
    eatGrowthItem = false;
    eatPoisonItem = false;
    direction = 'l';

    scoreGrowthItem = 0;
    scorePoisonItem = 0;
    snakeLength = 0;
    scoreGate = 0;

    srand(time(NULL));

    initGame();
    positionItem();
    drawWindow();
    drawSnake();
    printScore();

    refresh();
}

snakeGame::~snakeGame()
{
    nodelay(stdscr, false);
    getch();
    endwin();
}

void snakeGame::initGame()
{
    initscr(); // initialise the screen
    nodelay(stdscr, TRUE);
    keypad(stdscr, true); // initialise the keyboard: we can use arrows for directions
    noecho();             // user input is not displayed on the screen
    curs_set(0);          // cursor symbol is not not displayed on the screen (Linux)
    getmaxyx(stdscr, height, width);
}

void snakeGame::drawWindow()
{
    move(0, 0);
    addch(upLeftEdge);

    move(0, width - 12);
    addch(upRightEdge);

    move(height - 1, 0);
    addch(downLeftEdge);

    move(height - 1, width - 12);
    addch(downRightEdge);

    for (int i = 1; i < width - 12; i++) // draws top
    {
        move(0, i);
        addch(rowLine);
    }
    for (int i = 1; i < width - 12; i++) // draws bottom
    {
        move(height - 1, i);
        addch(rowLine);
    }
    for (int i = 1; i < height - 1; i++) // draws left side
    {
        move(i, 0);
        addch(colLine);
    }
    for (int i = 1; i < height - 1; i++) // draws right side
    {
        move(i, width - 12);
        addch(colLine);
    }
}

void snakeGame::drawSnake()
{
    for (int i = 0; i < 5; i++)
    {
        snake.push_back(Position(40 + i, 20));
    }
    for (int i = 0; i < snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(snakeBody);
    }
}

void snakeGame::printScore()
{
    move(0, width - 10);
    printw("Score Borad");
    move(1, width - 10);
    printw("B: (%d) / (%d)", (snakeLength, 10));
    move(2, width - 10);
    printw("+: (%d)", scoreGrowthItem);
    move(3, width - 10);
    printw("-: (%d)", scorePoisonItem);
    move(4, width - 10);
    printw("G: (%d)", scoreGate);
}

void snakeGame::positionItem()
{
    int numOfItem = rand() % 3 + 1;
    while (numOfItem > 0)
    {
        int setItem = rand() % 2 + 1;
        int px = rand() % width + 1;
        int py = rand() % height + 1;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == px && snake[i].y == py)
            {
                continue;
            }
        }

        for (int i = 0; i < item.size(); i++)
        {
            if (item[i].x == px && item[i].y == py)
            {
                continue;
            }
        }

        if (px >= width - 12 || py >= height - 1)
        {
            continue;
        }

        if (setItem == 1)
        {
            growthItem.x = px;
            growthItem.y = py;
            move(px, py);
            addch(growthItemChar);
        }
        else
        {
            poisonItem.x = px;
            poisonItem.y = py;
            move(px, py);
            addch(poisonItemChar);
        }
        refresh();
        numOfItem--;
    }
}

void snakeGame::snakePosition()
{
}

bool snakeGame::getGrowthItem()
{
    if (snake[0].x == growthItem.x && snake[0].y == growthItem.y)
    {
        snakeLength++;
        printScore();

        // if score is a multiple of 100, increase snake speed
        speed -= 1000;
        return eatGrowthItem = true;
    }
    else
    {
        return eatGrowthItem = false;
    }
}

bool snakeGame::getPoisonItem()
{
    if (snake[0].x == poisonItem.x && snake[0].y == poisonItem.y)
    {
        snakeLength--;
        printScore();

        // if score is a multiple of 100, increase snake speed
        speed -= 1000;
        return eatPoisonItem = true;
    }
    else
    {
        return eatPoisonItem = false;
    }
}

bool snakeGame::collision()
{
    if (snake[0].x == 0 || snake[0].x == width - 12 || snake[0].y == 0 || snake[0].y == height - 1)
    {
        return true;
    }

    for (int i = 2; i < snake.size(); i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return true;
        }
    }
    return false;
}

void snakeGame::randomGate()
{
}

void snakeGame::snakeMove()
{
    int KeyPressed = getch();
    switch (KeyPressed)
    {
    case KEY_LEFT:
        if (direction != 'r')
        {
            direction = 'l';
        }
        break;
    case KEY_RIGHT:
        if (direction != 'l')
        {
            direction = 'r';
        }
        break;
    case KEY_UP:
        if (direction != 'd')
        {
            direction = 'u';
        }
        break;
    case KEY_DOWN:
        if (direction != 'u')
        {
            direction = 'd';
        }
        break;
    case KEY_BACKSPACE:
        direction = 'q';
        break;
    }

    if (!eatGrowthItem)
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        refresh();
        snake.pop_back();
    }

    if (direction == 'l')
    {
        snake.insert(snake.begin(), Position(snake[0].x - 1, snake[0].y));
    }
    else if (direction == 'r')
    {
        snake.insert(snake.begin(), Position(snake[0].x + 1, snake[0].y));
    }
    else if (direction == 'u')
    {
        snake.insert(snake.begin(), Position(snake[0].x, snake[0].y - 1));
    }
    else if (direction == 'd')
    {
        snake.insert(snake.begin(), Position(snake[0].x, snake[0].y + 1));
    }

    move(snake[0].y, snake[0].x);
    addch(snakeBody);
    refresh();
    return;
}

void snakeGame::playGame()
{
    while (1)
    {
        if (collision())
        {
            move((height - 2) / 2, (width - 5) / 2);
            printw("GAME OVER");
            break;
        }

        getGrowthItem();
        getPoisonItem();
        snakeMove();

        if (direction == 'q')
        {
            break;
        }

        usleep(speed);
    }
}