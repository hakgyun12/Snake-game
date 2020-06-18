#include "fSnakeGame.h"
#include <unistd.h>


int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain(bool clear);
int AskUserToPlayContinue();
void ClearCentre();
int UserInput();

int main(){
    bool isClear;
    int level = 1;
    if (IsUserReady() == 'y')//사용자가 y를 입력하면 실행
    {
        do
        {
            snakeGame NewSnake(level++);
            NewSnake.PlayGame();
            isClear = NewSnake.getClear();
            if (!isClear)
            {
                level = 1;
            }
        } while (AskUserToPlayAgain(isClear) == 'y');
    }
    return 0;
}

void ClearCentre(float x, float y){
    clear(); //죽었을경우 화면을 깨끗하게 만든다.
    initscr();
    noecho();
    getmaxyx(stdscr, maxheight, maxwidth);
    move((maxheight / y), (maxwidth / x));
}

int UserInput(){
    int UserInput = getch(); //사용자입력대기
    refresh(); //스크린에 호출되기전까지 작업을 업데이트
    endwin(); //curse모드 종료
    clear();

    return UserInput;
}

//시작메뉴 출력
int IsUserReady(){
    ClearCentre(3.5, 2.5);
    printw("Welcome to the Snake Game. Are you ready? (y/n)");
    return UserInput();
}

// 재시작메뉴 출력
int AskUserToPlayAgain(bool clear){
    ClearCentre(2.8, 2.5);
    if (clear)
    {
        printw("Mission Complete! Do you want to play Continue? (y/n)");
    }
    else
    {
        printw("Do you want to play again? (y/n)");
    }
    return UserInput();
}
