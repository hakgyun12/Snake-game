#include "fSnakeGame.h"
#include <unistd.h>

using namespace std;

int KeyPressed;
CharPosition::CharPosition(int col, int row)
{
    x = col;
    y = row;
}

CharPosition::CharPosition()
{
    x = 0;
    y = 0;
}

snakeGame::snakeGame()
{
    // variables initialisation:
    partchar = 'x';       // character to represent the snake
    edgechar = (char)219; // full rectangle on the key table
    fruitchar = '+';
    poisonchar = '-';
    FruitTimer = 0;
    PoisonTimer = 0;
    fruit.x = 0;
    fruit.y = 0;
    poison.x = 0;
    poison.y = 0;
    currentLength = 3;
    maxLength = 3;
    scoreGrowthItem = 0;
    scorePoisonItem = 0;
    scoreGate = 0;
    del = 70000;
    speedChange = 50000;
    bool bEatsFruit = 0;
    bool bEatsPoison = 0;
    direction = 'l';
    srand(time(NULL));


    InitGameWindow();
    PositionFruit();
    PositionPoison();
    DrawWindow();
    DrawSnake();
    Print();

    refresh();
}

snakeGame::~snakeGame()
{
    nodelay(stdscr, false);
    getch();
    endwin();
}

// initialise the game window
void snakeGame::InitGameWindow()
{
    initscr(); // initialise the screen
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);                  // initialise the keyboard: we can use arrows for directions
    noecho();                              // user input is not displayed on the screen
    curs_set(0);                           // cursor symbol is not not displayed on the screen (Linux)
    getmaxyx(stdscr, maxheight, maxwidth); // define dimensions of game window
    return;
}

// draw the game window
void snakeGame::DrawWindow()
{
    for (int i = 0; i < maxwidth - 12; i++) // draws top
    {
        move(0, i);
        addch(edgechar);
    }

    for (int i = 0; i < maxwidth - 12; i++) // draws bottom
    {
        move(maxheight - 1, i);
        addch(edgechar);
    }

    for (int i = 0; i < maxheight - 1; i++) // draws left side
    {
        move(i, 0);
        addch(edgechar);
    }

    for (int i = 0; i < maxheight; i++) // draws right side
    {
        move(i, maxwidth - 12);
        addch(edgechar);
    }
    return;
}

// draw snake's body
void snakeGame::DrawSnake()
{
    for (int i = 0; i < 3; i++)
    {
        snake.push_back(CharPosition(30 + i, 10));
    }

    for (int i = 0; i < snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(partchar);
    }
    return;
}

// print score at bottom of window
void snakeGame::Print()
{
    move(0, maxwidth - 11);
    printw("Score Board");
    move(1, maxwidth - 11);
    printw("B:(%d)/(%d)", currentLength, maxLength);
    move(2, maxwidth - 11);
    printw("+:(%d)", scoreGrowthItem);
    move(3, maxwidth - 11);
    printw("-:(%d)", scorePoisonItem);
    move(4, maxwidth - 11);
    printw("G:(%d)", scoreGate);
}

// set game over situations
bool snakeGame::FatalCollision()
{
    // if the snake hits the edge of the window
    if (snake[0].x == 0 || snake[0].x == maxwidth - 12 || snake[0].y == 0 || snake[0].y == maxheight - 1)
    {
        return true;
    }

    // if the snake collides into himself
    for (int i = 2; i < snake.size(); i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return true;
        }
    }

    if (snake.size() < 3)
    {
        return true;
    }
    if (direction == 'r' && KeyPressed == KEY_LEFT)
    {
        return true;
    }
    if (direction == 'l' && KeyPressed == KEY_RIGHT)
    {
        return true;
    }
    if (direction == 'u' && KeyPressed == KEY_DOWN)
    {
        return true;
    }
    if (direction == 'd' && KeyPressed == KEY_UP)
    {
        return true;
    }

    return false;
}


// define behaviour when snake eats the fruit
bool snakeGame::GetsFruit()
{
    if (snake[0].x == fruit.x && snake[0].y == fruit.y)
    {
        FruitTimer = 0;
        PositionFruit();
        currentLength++;
        if (currentLength > maxLength)
        {
            maxLength = currentLength;
        }
        scoreGrowthItem++;
        Print();
        return bEatsFruit = true;
    }
    else
    {
        return bEatsFruit = false;
    }
}

// 
void snakeGame::PositionFruit()
{
    int tmpx = rand() % (maxwidth - 13) + 1; // 랜덤한 좌표를 생성
    int tmpy = rand() % (maxheight - 2) + 1;

    fruit.x = tmpx;
    fruit.y = tmpy;
  
    //뱀위치에 생성 x
    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == tmpx && snake[i].y == tmpy)
        {
            fruit.x = tmpx;
            fruit.y = tmpy;
            continue; 
        }
    }
    move(fruit.y, fruit.x);
    addch(fruitchar);
    refresh();
}

void snakeGame::FruitTime(){
    FruitTimer++;
    if (FruitTimer % 100 == 0){  
        move(fruit.y, fruit.x);
        printw(" ");
        PositionFruit();
        FruitTimer = 0;
    }
}

void snakeGame::PoisonTime(){
    PoisonTimer++;
    if (PoisonTimer % 100 == 0){  
        move(poison.y, poison.x);
        printw(" ");
        PositionPoison();
        PoisonTimer = 0;
    }
}


bool snakeGame::GetsPoison()
{
    if (snake[0].x == poison.x && snake[0].y == poison.y)
    {
        PoisonTimer = 0;
        PositionPoison();
        currentLength--;
        scorePoisonItem++;
        Print();
        return bEatsPoison = true;
    }
    else
    {
        return bEatsPoison = false;
    }
}
void snakeGame::PositionPoison()
{
    int tmpx1 = rand() % (maxwidth - 13) + 1; // +1 to avoid the 0
    int tmpy1 = rand() % (maxheight - 2) + 1;
    poison.x = tmpx1; //랜덤위치 아이템
    poison.y = tmpy1;

    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == tmpx1 && snake[i].y == tmpy1)
        {
            poison.x = tmpx1;
            poison.y = tmpy1;
        }
    }
    move(poison.y, poison.x);

    addch(poisonchar);
    refresh();
}

// define snake's movements
void snakeGame::MoveSnake()
{
    KeyPressed = getch();
    switch (KeyPressed)
    {
    case KEY_LEFT:
        if (direction != 'r')
        {
            direction = 'l';
        }
        else
            FatalCollision();
        break;
    case KEY_RIGHT:
        if (direction != 'l')
        {
            direction = 'r';
        }
        else
            FatalCollision();
        break;
    case KEY_UP:
        if (direction != 'd')
        {
            direction = 'u';
        }
        else
            FatalCollision();
        break;
    case KEY_DOWN:
        if (direction != 'u')
        {
            direction = 'd';
        }
        else
            FatalCollision();
        break;
    case KEY_BACKSPACE:
        direction = 'q'; // key to quit the game
        break;
    }

    // the snake doesn't eat fruit, remains same size
    if (!(bEatsFruit || bEatsPoison))
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x); // moves at the end of the tail
        printw(" ");                                                // add empty ch to remove last character
        refresh();
        snake.pop_back(); // removes the last element in the vector, reducing the container size by one
    }
    else if (bEatsPoison)
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        snake.pop_back();
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        refresh();
        snake.pop_back();
    }

    // the snake moves and we add an extra character at the beginning of the vector
    // add a head and initialise new coordinates for CharPosition according to the direction input
    if (direction == 'l')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x - 1, snake[0].y));
    }
    else if (direction == 'r')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x + 1, snake[0].y));
    }
    else if (direction == 'u')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y - 1));
    }
    else if (direction == 'd')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y + 1));
    }

    // move to the new CharPosition coordinates
    move(snake[0].y, snake[0].x);
    addch(partchar); // add a new head
    refresh();
    return;
}

void snakeGame::PlayGame()
{

    while (1)
    {
        clock_t start = clock();
        if (FatalCollision())
        {
            move((maxheight - 2) / 2, (maxwidth - 5) / 2);
            printw("GAME OVER");
            endwin();
            break;
        }
        FruitTime();
        PoisonTime();
        GetsFruit();
        GetsPoison();
        MoveSnake();
        if (direction == 'q') //exit
        {
            break;
        }
        usleep(del); // delay
    }
}