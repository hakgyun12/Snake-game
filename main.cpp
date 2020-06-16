/*
This is the console executable, that makes use of the snakeGame class.
This is handling all user interaction. For game logic, please see snakeGame.h.
*/

#include "snakeGame.h"

// Unreal standards

int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain(bool clear);
int AskUserToPlayContinue();
void ClearCentre();
int UserInput();

int main()
{
    bool isClear;
    int level = 1;
    if (IsUserReady() == 'y') // wait for confirmation of the user
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

// clear the screen and centre the cursor
void ClearCentre(float x, float y)
{
    clear(); // clear the screen if the game is played for the 2nd time
    initscr();
    noecho();
    getmaxyx(stdscr, maxheight, maxwidth);
    move((maxheight / y), (maxwidth / x));
}

// receive user confirmation
int UserInput()
{
    int UserInput = getch();
    refresh();
    endwin();
    clear();

    return UserInput;
}

// print start menu
int IsUserReady()
{
    ClearCentre(3.5, 2.5);
    printw("Welcome to the Snake Game. Are you ready? (y/n)");
    return UserInput();
}

// print end of the game menu and ask user to play again
int AskUserToPlayAgain(bool clear)
{
    ClearCentre(2.8, 2.5);
    if (clear)
    {
        printw("Do you want to play Continue? (y/n)");
    }
    else
    {
        printw("Do you want to play again? (y/n)");
    }
    return UserInput();
}