#include "fSnakeGame.h"


int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();

int main()
{
    if (IsUserReady() == 'y') //사용자가 y를 입력하면 실행
        do
        {
            {
                snakeGame NewSnake; //객체생성
                NewSnake.PlayGame();
            }
        } while (AskUserToPlayAgain() == 'y');
    return 0;
}

void ClearCentre(float x, float y)
{
    clear(); //죽었을경우 화면을 깨끗하게 만든다.
    initscr();
    noecho();
    getmaxyx(stdscr, maxheight, maxwidth);
    move((maxheight / y), (maxwidth / x));
}

int UserInput()
{
    int UserInput = getch(); //사용자입력대기
    refresh(); //스크린에 호출되기전까지 작업을 업데이트
    endwin(); //curse모드 종료
    clear();

    return UserInput;
}

//시작메뉴 출력
int IsUserReady()
{
    ClearCentre(3, 2.5);
    printw("Welcome to the Snake Game. Are you ready? (y/n)");
    return UserInput();
}

// 재시작메뉴 출력
int AskUserToPlayAgain()
{
    ClearCentre(2.5, 2.5);
    printw("Do you want to play again? (y/n)");
    return UserInput();
}