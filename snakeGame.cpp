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
    partchar = 'x';       // 뱀의 몸 표시
    edgechar = 'o';       // 벽 표시
    growthItemchar = '+'; // 성장 아이템 표시
    poisonItemchar = '-'; // 감소 아이템 표시
    speedItemchar = '*';  // 스피드 아이템 표시
    growthItem.x = 0;     // 성장 아이템이 놓일 x좌표
    growthItem.y = 0;     // 성장 아이템이 놓일 y좌표
    poisonItem.x = 0;     // 감소 아이템이 놓일 x좌표
    poisonItem.y = 0;     // 감소 아이템이 놓일 y좌표
    speedItem.x = 0;      // 스피드 아이템이 놓일 x좌표
    speedItem.y = 0;      // 스피드 아이템이 놓일 y좌표
    currentLength = 3;    // 뱀의 현재 길이
    isClear = false;

    requiredLength = 3 * level;     // 다음 단계로 넘어가기 위해 만족해야 할 뱀의 길이
    requiredGrowthItem = 1 * level; // 다음 단계로 넘어가기 위해 만족해야 할 Growth Item 먹은 수
    requiredPoisonItem = 0 * level; // 다음 단계로 넘어가기 위해 만족해야 할 Poison Item 먹은 수
    requiredGate = 1 * level;       // 다음 단계로 넘어가기 위해 만족해야 할 Gate 통과 횟수

    scoreGrowthItem = 0; // 성장 아이템을 획득한 수
    scorePoisonItem = 0; // 감소 아이템을 획득한 수
    scoreGate = 0;       // 게이트를 통과한 수
    speed = 50000;       // 뱀의 속도
    itemChange = 80;     // 뱀이 아무것도 먹지 않을 때 아이템 위치가 대기하는 시간
    gateChange = 100;    // 게이트가 유지되는 시간
    bEatsGrowth = 0;     // 뱀이 성장 아이템을 획득했는지
    bEatsPoison = 0;     // 뱀이 감소 아이템을 획득했는지
    bAtGate_1 = 0;       // 뱀이 게이트1를 통과했는지
    bAtGate_2 = 0;       // 뱀이 게이트2를 통과했는지
    direction = 'l';     // 뱀의 진행 방향
    growthItemTimer = 0; // 성장 아이템이 유지되는 시간
    poisonItemTimer = 0; // 감소 아이템이 유지되는 시간
    speedItemTimer = 0;  // 스피트 아이템이 유지되는 시간
    gateTimer = 0;       // 게이트가 유지되는 시간
    srand(time(NULL));

    InitGameWindow(level);
    PositionGrowth();
    PositionPoison();
    PositionSpeed();
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
void snakeGame::InitGameWindow(int level)
{
    initscr(); // initialise the screen
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);                  // initialise the keyboard: we can use arrows for directions
    noecho();                              // user input is not displayed on the screen
    curs_set(0);                           // cursor symbol is not not displayed on the screen (Linux)
    getmaxyx(stdscr, maxheight, maxwidth); // define dimensions of game window
    maxheight -= (level - 1) * 5;
    maxwidth -= (level - 1) * 10;
    return;
}

// draw the game window
void snakeGame::DrawWindow()
{
    for (int i = 1; i < maxwidth - 12; i++) // 위의 벽
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

    for (int i = 1; i < maxwidth - 12; i++) // 아래 벽
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

    for (int i = 1; i < maxheight - 1; i++) // 왼쪽 벽
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

    for (int i = 1; i < maxheight - 1; i++) // 오른쪽 벽
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
    for (int i = 15; i < maxheight / 2 + 2; i++) // 중간벽 ㅣ
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

    for (int i = (maxwidth - 12) / 2 - 10; i < maxwidth / 2 + 1; i++) // 중간벽 ㅡ
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

// 우측에 표시할 점수판과 미션판
void snakeGame::PrintScore()
{
    move(0, maxwidth - 11);
    printw("Score Board");
    move(1, maxwidth - 11);
    printw("B:(%d)", currentLength); // 현재 뱀의 길이 표시
    move(2, maxwidth - 11);
    printw("+:(%d)", scoreGrowthItem); // 뱀이 성장 아이템 획득 수 표시
    move(3, maxwidth - 11);
    printw("-:(%d)", scorePoisonItem); // 뱀이 감소 아이템 획득 수 표시
    move(4, maxwidth - 11);
    printw("G:(%d)", scoreGate); // 뱀이 게이트를 통과한 수 표시

    move(6, maxwidth - 11);
    printw("Mission");
    move(7, maxwidth - 11);
    if (requiredLength - currentLength <= 0) // 충족 길이를 만족하면 v표시
    {
        printw("B:(%s)", "V");
    }
    else
    {
        printw("B:(%d)", requiredLength - currentLength); // 그렇지 않으면 충족 길이까지 남은 수 표시
    }
    move(8, maxwidth - 11);
    if (requiredGrowthItem - scoreGrowthItem <= 0) // 성장 아이템 획득 횟수를 만족하면 v표시
    {
        printw("+:(%s)", "V");
    }
    else
    {
        printw("+:(%d)", requiredGrowthItem - scoreGrowthItem); // 그렇지 않으면 충족 횟수까지 남은 수 표시
    }
    move(9, maxwidth - 11);
    if (requiredPoisonItem - scorePoisonItem <= 0) // 감소 아이템 획득 횟수를 만족하면 v표시
    {

        printw("-:(%s)", "V");
    }
    else
    {
        printw("-:(%d)", requiredPoisonItem - scorePoisonItem); // 그렇지 않으면 충족 횟수까지 남은 수 표시
    }
    move(10, maxwidth - 11);
    if (requiredGate - scoreGate <= 0) // 게이트 통과 횟수를 만족하면 v표시
    {

        printw("G:(%s)", "V");
    }
    else
    {
        printw("G:(%d)", requiredGate - scoreGate); // 그렇지 않으면 충족 통과 횟수까지 남은 수 표시
    }
}

// 다음 stage로 넘어갈 수 있는지
bool snakeGame::NextStage()
{
    if (currentLength >= requiredLength && scoreGrowthItem >= requiredGrowthItem && scorePoisonItem >= requiredPoisonItem && scoreGate >= requiredGate)
    {
        return true; // 길이, 성장아이템 획득 횟수, 감소 아이템 획득 횟수, 게이트 통과 횟수를 모두 충족하면 true
    }
    return false;
}

// 게이트 위치 배치
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
    init_pair(5, COLOR_BLUE, COLOR_BLUE); // 게이트 색 표시
    attron(COLOR_PAIR(5));
    move(gate_1.y, gate_1.x);
    addch(edgechar);
    move(gate_2.y, gate_2.x);
    addch(edgechar);
    attroff(COLOR_PAIR(5));
    refresh();
}

// 게이트 시간
void snakeGame::gateTime()
{
    gateTimer++;
    if (gateTimer % gateChange == 0) // 일정 시간이 지나면 원래 게이트는 사라지고 새로운 곳에 게이트가 생김
    {                                // 그리고 다시 타이머는 0으로 초기화
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

// 성장 아이템 배치
void snakeGame::PositionGrowth()
{
    int tmpx, tmpy;     // 아이템 위치
    bool clear = false; // 아이템이 놓일 위치가 타당한지
    while (!clear)
    {
        tmpx = rand() % (maxwidth - 13) + 1; // 놓일 위치가 타당해질 때까지 무작위로 아이템이 놓일 위치 선정
        tmpy = rand() % (maxheight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy) // 뱀의 몸에 아이템이 생기는지
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++)
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy) // 벽에 아이템이 생기는지
            {
                clear = false;
                break;
            }
        }
    }

    // 성장 아이템의 색 표시
    growthItem.x = tmpx;
    growthItem.y = tmpy;
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    move(growthItem.y, growthItem.x);
    addch(growthItemchar);
    attroff(COLOR_PAIR(1));
    refresh();
}

// 성장 아이템 시간
void snakeGame::growthItemTime()
{
    growthItemTimer++;
    if (growthItemTimer % itemChange == 0) // 일정 시간이 지나면 원래 있던 성장 아이템이 사라지고 새로운 곳에 성장 아이템이 생김
    {                                      // 그리고 다시 타이머는 0으로 초기화
        move(growthItem.y, growthItem.x);
        printw(" ");
        PositionGrowth();
        growthItemTimer = 0;
    }
}

// 감소 아이템 배치
void snakeGame::PositionPoison()
{
    int tmpx, tmpy;     // 아이템 위치
    bool clear = false; // 아이템이 놓일 곳이 타당한지
    while (!clear)
    {
        tmpx = rand() % (maxwidth - 13) + 1; // 놓일 위치가 타당해질 때까지 무작위로 아이템이 놓일 위치 선정
        tmpy = rand() % (maxheight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy) // 뱀의 몸에 아이템이 생기는지
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++)
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy) // 벽에 아이템이 생기는지
            {
                clear = false;
                break;
            }
        }
    }

    // 감소 아이템의 색 표시
    poisonItem.x = tmpx;
    poisonItem.y = tmpy;
    start_color();
    init_pair(2, COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(2));
    move(poisonItem.y, poisonItem.x);
    addch(poisonItemchar);
    attroff(COLOR_PAIR(2));
    refresh();
}

// 감소 아이템 시간
void snakeGame::poisonItemTime()
{
    poisonItemTimer++;
    if (poisonItemTimer % itemChange == 0) // 일정 시간이 지나면 원래 있던 감소 아이템이 사라지고 새로운 곳에 감소 아이템이 생김
    {                                      // 그리고 다시 타이머는 0으로 초기화
        move(poisonItem.y, poisonItem.x);
        printw(" ");
        PositionPoison();
        poisonItemTimer = 0;
    }
}

// 스피드 아이템 배치
void snakeGame::PositionSpeed()
{
    int tmpx, tmpy;     // 아이템 위치
    bool clear = false; // 아이템이 놓일 곳이 타당한지
    while (!clear)
    {
        tmpx = rand() % (maxwidth - 13) + 1; // 놓일 위치가 타당할 때까지 무작위로 아이템이 놓일 위치 선정
        tmpy = rand() % (maxheight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy) // 뱀의 몸에 아이템이 생기는지
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++) // 벽에 아이템이 생기는지
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }
    }
    // 스피드 아이템 색 표시
    speedItem.x = tmpx;
    speedItem.y = tmpy;
    start_color();
    init_pair(6, COLOR_WHITE, COLOR_YELLOW);
    attron(COLOR_PAIR(6));
    move(speedItem.y, speedItem.x);
    addch(speedItemchar);
    attroff(COLOR_PAIR(6));
    refresh();
}

void snakeGame::speedItemTime() // poisonItem의 위치가 바뀜
{
    speedItemTimer++;
    if (speedItemTimer % itemChange == 0) // 일정 시간이 지나면 원래 스피드 아이템은 사라지고 다른 곳에 생성됨
    {                                     // 타이머는 0으로 초기화
        move(speedItem.y, speedItem.x);
        printw(" ");
        PositionSpeed();
        speedItemTimer = 0;
    }
}

// set game over situations
bool snakeGame::FatalCollision() // 뱀이 벽에 부딫혔을 때 종료
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

    // 뱀이 자신의 몸에 부딫힌 경우 종료
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
    // 진행방향의 반대키를 누른경우 종료
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

// 스피드 아이템을 획득했을 때
void snakeGame::GetsSpeed()
{
    if (snake[0].x == speedItem.x && snake[0].y == speedItem.y)
    {
        speedItemTimer = 0;
        PositionSpeed();
        speed -= 10000;
    }
}

// 게이트를 통과했을 때
bool snakeGame::GetsGate()
{
    if (snake[0].x == gate_1.x && snake[0].y == gate_1.y)
    {
        gateTimer = gateChange - snake.size() - 1;
        scoreGate++;  // 게이트 통과 횟수 증가
        PrintScore(); // 점수판에 게이트 통과 횟수 표시
        return bAtGate_1 = true;
    }
    else if (snake[0].x == gate_2.x && snake[0].y == gate_2.y)
    {
        gateTimer = gateChange - snake.size() - 1;
        scoreGate++;  // 게이트 통과 횟수 증가
        PrintScore(); // 점수판에 게이트 통과 횟수 표시
        return bAtGate_2 = true;
    }
}

// 성장 아이템을 획득했을 때
bool snakeGame::GetsGrowth()
{
    if (snake[0].x == growthItem.x && snake[0].y == growthItem.y)
    {
        growthItemTimer = 0; // 성장 아이템 타이머 0으로 초기화
        PositionGrowth();    // 성장 아이템을 다른 위치에 배치
        currentLength++;     // 현재 길이 증가
        scoreGrowthItem++;   // 점수판에 표시되는 성장 아이템 점수 증가
        PrintScore();        // 점수판에 표시
        return bEatsGrowth = true;
    }
    else
    {
        return bEatsGrowth = false;
    }
}

// 감소 아이템을 획득했을 때
bool snakeGame::GetsPoison()
{
    if (snake[0].x == poisonItem.x && snake[0].y == poisonItem.y)
    {
        poisonItemTimer = 0; // 감소 아이템 타이머 0으로 초기화
        PositionPoison();    // 감소 아이템을 다른 위치에 배치
        currentLength--;     // 현재 길이 감소
        scorePoisonItem++;   // 점수판에 표시되는 성장 아이템 점수 증가
        PrintScore();        // 점수판에 표시
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

    // 각각의 방향의 블럭들이 wall인지 검사
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

    // 절대 벽을 위한 케이스 고려
    if ((leftBlock.x == 0 && leftBlock.y == 0) || (leftBlock.x == 0 && leftBlock.y == maxheight - 1))
    {
        isLeftWall = true;
    }
    if ((rightBlock.x == maxwidth - 12 && rightBlock.y == 0) || (rightBlock.x == maxwidth - 12 && rightBlock.y == maxheight - 1))
    {
        isRightWall = true;
    }
    if ((upBlock.x == 0 && upBlock.y == 0) || (upBlock.x == maxwidth - 12 && upBlock.y == 0))
    {
        isUpWall = true;
    }
    if ((downBlock.x == 0 && downBlock.y == maxheight - 1) || (downBlock.x == maxwidth - 12 && downBlock.y == maxheight - 1))
    {
        isDownWall = true;
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

    // 들어온 방향에 따라 나가는 방향을 지정(시계방향)
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

// 뱀 움직임
void snakeGame::MoveSnake()
{
    KeyPressed = getch();
    // 각 방향키를 눌렀을 경우와 그에 따른 방향과 역방향인 경우
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
        direction = 'q';
        break;
    }

    // 뱀이 아무것도 획득하지 않았을 때(조건문 끝나고 다시 추가해줌)
    if (!(bEatsGrowth || bEatsPoison))
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x); // 꼬리 부분 감소
        printw(" ");
        refresh();
        snake.pop_back(); // 뱀 벡터에서 1개 제외
    }
    else if (bEatsPoison) // 뱀이 poisonItem을 먹었을 때 길이 감소
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        snake.pop_back();
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" "); // 위의 아무것도 획득하지 않은 상태에 한 번 더 감소함
        refresh();
        snake.pop_back();
    }

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

    // 성장 아이템을 먹거나 아무것도 먹지 않았을 때
    move(snake[0].y, snake[0].x);
    addch(partchar); // 뱀의 머리에 추가
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
        GetsSpeed();
        growthItemTime();
        poisonItemTime();
        speedItemTime();
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