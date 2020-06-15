#include "snakeGame.h"
#include <unistd.h>

using namespace std;

// Unreal coding standards

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

snakeGame::snakeGame(int level)
{
    // variables initialisation:
    partchar = 'x'; // character to represent the snake
    edgechar = 'o'; // full rectangle on the key table
    growthItemchar = '+';
    poisonItemchar = '-';
    growthItem.x = 0;
    growthItem.y = 0;
    poisonItem.x = 0;
    poisonItem.y = 0;
    currentLength = 3;
    isClear = false;

    requiredLength = 0 * level;     // 다음 단계로 넘어가기 위해 만족해야 할 뱀의 길이
    requiredGrowthItem = 1 * level; // 다음 단계로 넘어가기 위해 만족해야 할 Growth Item 먹은 수
    requiredPoisonItem = 0 * level; // 다음 단계로 넘어가기 위해 만족해야 할 Poison Item 먹은 수
    requiredGate = 0 * level;       // 다음 단계로 넘어가기 위해 만족해야 할 Gate 통과 횟수

    scoreGrowthItem = 0;
    scorePoisonItem = 0;
    scoreGate = 0;
    speed = 100000;
    itemChange = 80; // 뱀이 아무것도 먹지 않을 때 아이템 위치가 대기하는 시간
    gateChange = 100;
    bEatsGrowth = 0;
    bEatsPoison = 0;
    bAtGate_1 = 0;
    bAtGate_2 = 0;
    direction = 'l';
    growthItemTimer = 0;
    poisonItemTimer = 0;
    gateTimer = 0;
    srand(time(NULL));

    InitGameWindow();
    PositionGrowth();
    PositionPoison();
    DrawWindow();
    DrawSnake();
    PrintScore();
    PositionGate();

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
    for (int i = 1; i < maxwidth - 12; i++) // draws top
    {
        wall.push_back(CharPosition(i, 0));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(0, i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxwidth - 12; i++) // draws bottom
    {
        wall.push_back(CharPosition(i, maxheight - 1));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(maxheight - 1, i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxheight - 1; i++) // draws left side
    {
        wall.push_back(CharPosition(0, i));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(i, 0);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxheight - 1; i++) // draws right side
    {
        wall.push_back(CharPosition(maxwidth - 12, i));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(i, maxwidth - 12);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }
    for (int i = 15; i < maxheight / 2 + 2; i++) // draws "중간벽 ㅣ"
    {
        wall.push_back(CharPosition((maxwidth - 12) / 2 - 10, i));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(i, (maxwidth - 12) / 2 - 10);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = (maxwidth - 12) / 2 - 10; i < maxwidth / 2 + 1; i++) // draws "중간벽 ㅡ"
    {
        wall.push_back(CharPosition(i, maxheight / 2 + 1));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(maxheight / 2 + 1, i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    // 모서리 부분을 다른 색으로 표시
    start_color();
    init_pair(4, COLOR_BLACK, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    move(0, 0);
    addch(edgechar);
    move(0, maxwidth - 12);
    addch(edgechar);
    move(maxheight - 1, 0);
    addch(edgechar);
    move(maxheight - 1, maxwidth - 12);
    addch(edgechar);
    attroff(COLOR_PAIR(4));
    refresh();
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
void snakeGame::PrintScore()
{
    move(0, maxwidth - 11);
    printw("Score Board");
    move(1, maxwidth - 11);
    printw("B:(%d)", currentLength);
    move(2, maxwidth - 11);
    printw("+:(%d)", scoreGrowthItem);
    move(3, maxwidth - 11);
    printw("-:(%d)", scorePoisonItem);
    move(4, maxwidth - 11);
    printw("G:(%d)", scoreGate);

    move(6, maxwidth - 11);
    printw("Mission");
    move(7, maxwidth - 11);
    printw("B:(%d)", requiredLength - currentLength);
    move(8, maxwidth - 11);
    printw("+:(%d)", requiredGrowthItem - scoreGrowthItem);
    move(9, maxwidth - 11);
    printw("-:(%d)", requiredPoisonItem - scorePoisonItem);
    move(10, maxwidth - 11);
    printw("G:(%d)", requiredGate - scoreGate);
}

bool snakeGame::NextStage()
{
    if (currentLength >= requiredLength && scoreGrowthItem >= requiredGrowthItem && scorePoisonItem >= requiredPoisonItem && scoreGate >= requiredGate)
    {
        return true;
    }
    return false;
}

void snakeGame::PositionGate()
{
    // gate 백터 내에서 난수로 index를 중복 안되게 받아옴
    int gate_idx1 = rand() % wall.size();
    int gate_idx2 = rand() % wall.size();
    while (gate_idx2 == gate_idx1)
    {
        gate_idx2 = rand() % wall.size();
    }
    gate_1 = wall[gate_idx1];
    gate_2 = wall[gate_idx2];
    start_color();
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    attron(COLOR_PAIR(5));
    move(gate_1.y, gate_1.x);
    addch(edgechar);
    move(gate_2.y, gate_2.x);
    addch(edgechar);
    attroff(COLOR_PAIR(5));
    refresh();
}

void snakeGame::gateTime()
{
    gateTimer++;
    if (gateTimer % gateChange == 0)
    {
        attron(COLOR_PAIR(3));
        move(gate_1.y, gate_1.x);
        addch(edgechar);
        move(gate_2.y, gate_2.x);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
        PositionGate();
        gateTimer = 0;
    }
}

// position a new growthItem in the game window
void snakeGame::PositionGrowth()
{
    int tmpx = rand() % (maxwidth - 13) + 1; // +1 to avoid the 0
    int tmpy = rand() % (maxheight - 2) + 1;

    growthItem.x = tmpx;
    growthItem.y = tmpy;

    // check that the growthItem is not positioned on the snake
    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == tmpx && snake[i].y == tmpy)
        {
            growthItem.x = tmpx;
            growthItem.y = tmpy;
            // if true, ignore the following and go back to the beginning of function
        }
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    move(growthItem.y, growthItem.x);
    addch(growthItemchar);
    attroff(COLOR_PAIR(1));
    refresh();
}

void snakeGame::growthItemTime()
{
    growthItemTimer++;
    if (growthItemTimer % itemChange == 0) // growthItem의 위치가 바뀜
    {
        move(growthItem.y, growthItem.x);
        printw(" ");
        PositionGrowth();
        growthItemTimer = 0;
    }
}

void snakeGame::PositionPoison()
{
    int tmpx1 = rand() % (maxwidth - 13) + 1; // +1 to avoid the 0
    int tmpy1 = rand() % (maxheight - 2) + 1;

    poisonItem.x = tmpx1;
    poisonItem.y = tmpy1;

    for (int i = 0; i < snake.size(); i++)
    {
        if (snake[i].x == tmpx1 && snake[i].y == tmpy1)
        {
            poisonItem.x = tmpx1;
            poisonItem.y = tmpy1;
        }
    }
    start_color();
    init_pair(2, COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(2));
    move(poisonItem.y, poisonItem.x);
    addch(poisonItemchar);
    attroff(COLOR_PAIR(2));
    refresh();
}

void snakeGame::poisonItemTime() // poisonItem의 위치가 바뀜
{
    poisonItemTimer++;
    if (poisonItemTimer % itemChange == 0)
    {
        move(poisonItem.y, poisonItem.x);
        printw(" ");
        PositionPoison();
        poisonItemTimer = 0;
    }
}

// set game over situations
bool snakeGame::FatalCollision() // 이름 바꿔야 할 듯, 스네이크 길이 미만 조건도 포함되어 있으니...
{
    for (int i = 0; i < wall.size(); i++)
    {
        if (snake[0].x == wall[i].x && snake[0].y == wall[i].y)
        {
            if (!((snake[0].x == gate_1.x && snake[0].y == gate_1.y) || (snake[0].x == gate_2.x && snake[0].y == gate_2.y)))
            {
                return true;
                break;
            }
        }
    }

    // if the snake collides into himself
    for (int i = 2; i < snake.size(); i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return true;
        }
    }

    if (snake.size() < 3) // 뱀의 길이가 3보다 짧아진 경우 종료
    {
        return true;
    }
    if (direction == 'r' && KeyPressed == KEY_LEFT) // 진행방향의 반대키를 누른경우 종료
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

bool snakeGame::GetsGate()
{
    if (snake[0].x == gate_1.x && snake[0].y == gate_1.y)
    {
        gateTimer = gateChange - snake.size() - 1;
        scoreGate++;
        PrintScore();
        return bAtGate_1 = true;
    }
    else if (snake[0].x == gate_2.x && snake[0].y == gate_2.y)
    {
        gateTimer = gateChange - snake.size() - 1;
        scoreGate++;
        PrintScore();
        return bAtGate_2 = true;
    }
}

// define behaviour when snake eats the growthItem
bool snakeGame::GetsGrowth()
{
    if (snake[0].x == growthItem.x && snake[0].y == growthItem.y)
    {
        growthItemTimer = 0;
        PositionGrowth();
        currentLength++;
        /*
        if (currentLength >= requiredLength){   
        // 다음 단계로 진행하는 조건 하나 만족
        }
        */
        scoreGrowthItem++;
        PrintScore();
        return bEatsGrowth = true;
    }
    else
    {
        return bEatsGrowth = false;
    }
}

bool snakeGame::GetsPoison()
{
    if (snake[0].x == poisonItem.x && snake[0].y == poisonItem.y)
    {
        poisonItemTimer = 0;
        PositionPoison();
        currentLength--;
        scorePoisonItem++;
        PrintScore();
        return bEatsPoison = true;
    }
    else
    {
        return bEatsPoison = false;
    }
}

char snakeGame::getWarpDirection(char d, CharPosition gate)
{
    char result;
    // 매개변수로 들어오는 gate에 좌측, 위쪽, 우측, 아래쪽의 CharPosition 객체를 생성
    CharPosition leftBlock(gate.x - 1, gate.y);
    CharPosition rightBlock(gate.x + 1, gate.y);
    CharPosition upBlock(gate.x, gate.y - 1);
    CharPosition downBlock(gate.x, gate.y + 1);

    // 각각의 방향의 블럭들이 wall인지 아닌지 판단하는 bool 변수 선언
    bool isLeftWall = false;
    bool isRightWall = false;
    bool isUpWall = false;
    bool isDownWall = false;

    for (int i = 0; i < wall.size(); i++)
    {
        if (wall[i].x == leftBlock.x && wall[i].y == leftBlock.y)
        {
            isLeftWall = true;
        }
        if (wall[i].x == rightBlock.x && wall[i].y == rightBlock.y)
        {
            isRightWall = true;
        }
        if (wall[i].x == upBlock.x && wall[i].y == upBlock.y)
        {
            isUpWall = true;
        }
        if (wall[i].x == downBlock.x && wall[i].y == downBlock.y)
        {
            isDownWall = true;
        }
    }

    // 가장자리인지 판별
    if (gate.x == 0)
    {
        isLeftWall = true;
    }
    if (gate.x == maxwidth - 12)
    {
        isRightWall = true;
    }
    if (gate.y == 0)
    {
        isUpWall = true;
    }
    if (gate.y == maxheight - 1)
    {
        isDownWall = true;
    }

    // 들어온 방향에 따라 나가는 방향을 지정
    if (d == 'l')
    {
        if (!isLeftWall)
        {
            result = 'l';
        }
        else if (!isUpWall)
        {
            result = 'u';
        }
        else if (!isRightWall)
        {
            result = 'r';
        }
        else
        {
            result = 'd';
        }
    }

    if (d == 'u')
    {
        if (!isUpWall)
        {
            result = 'u';
        }
        else if (!isRightWall)
        {
            result = 'r';
        }
        else if (!isDownWall)
        {
            result = 'd';
        }
        else
        {
            result = 'l';
        }
    }

    if (d == 'r')
    {
        if (!isRightWall)
        {
            result = 'r';
        }
        else if (!isDownWall)
        {
            result = 'd';
        }
        else if (!isLeftWall)
        {
            result = 'l';
        }
        else
        {
            result = 'u';
        }
    }

    if (d == 'd')
    {
        if (!isDownWall)
        {
            result = 'd';
        }
        else if (!isLeftWall)
        {
            result = 'l';
        }
        else if (!isUpWall)
        {
            result = 'u';
        }
        else
        {
            result = 'r';
        }
    }
    return result;
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

    // the snake doesn't eat growthItem, remains same size
    if (!(bEatsGrowth || bEatsPoison))
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x); // moves at the end of the tail
        printw(" ");                                                // add empty ch to remove last character
        refresh();
        snake.pop_back(); // removes the last element in the vector, reducing the container size by one
    }
    else if (bEatsPoison) // 뱀이 poisonItem을 먹었을 때 길이 감소
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

    if (bAtGate_1)
    {
        bAtGate_1 = false;
        direction = getWarpDirection(direction, gate_2);
        if (direction == 'r')
        {
            snake[0] = CharPosition(gate_2.x + 1, gate_2.y);
        }
        else if (direction == 'l')
        {
            snake[0] = CharPosition(gate_2.x - 1, gate_2.y);
        }
        else if (direction == 'd')
        {
            snake[0] = CharPosition(gate_2.x, gate_2.y + 1);
        }
        else if (direction == 'u')
        {
            snake[0] = CharPosition(gate_2.x, gate_2.y - 1);
        }
    }
    else if (bAtGate_2)
    {
        bAtGate_2 = false;
        direction = getWarpDirection(direction, gate_1);
        if (direction == 'r')
        {
            snake[0] = CharPosition(gate_1.x + 1, gate_1.y);
        }
        else if (direction == 'l')
        {
            snake[0] = CharPosition(gate_1.x - 1, gate_1.y);
        }
        else if (direction == 'd')
        {
            snake[0] = CharPosition(gate_1.x, gate_1.y + 1);
        }
        else if (direction == 'u')
        {
            snake[0] = CharPosition(gate_1.x, gate_1.y - 1);
        }
    }

    // move to the new CharPosition coordinates
    move(snake[0].y, snake[0].x);
    addch(partchar); // add a new head
    refresh();
    return;
}

bool snakeGame::getClear()
{
    return isClear;
}

void snakeGame::PlayGame()
{
    isClear = true;
    while (!NextStage())
    {
        if (FatalCollision())
        {
            move((maxheight - 2) / 2, (maxwidth - 20) / 2);
            printw("GAME OVER");
            endwin();
            isClear = false;
            break;
        }
        GetsGrowth();
        GetsPoison();
        GetsGate();
        growthItemTime();
        poisonItemTime();
        gateTime();
        MoveSnake();
        if (direction == 'q') //exit
        {
            break;
        }
        usleep(speed); // delay
    }
    if (isClear)
    {
        move((maxheight - 2) / 2, (maxwidth - 30) / 2);
        printw("Mission Complete");
        endwin();
    }
}