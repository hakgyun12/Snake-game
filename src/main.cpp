/*
This is the console executable, that makes use of the snakeGame class.
This is handling all user interaction. For game logic, please see snakeGame.h.
*/

#include "snakeGame.h"
#include <unistd.h>

// Unreal standards

int maxheight, maxwidth;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
int AskUserToPlayContinue();
void ClearCentre();
int UserInput();

int main()
{
    int level = 1;
    bool checked = true;
    if (IsUserReady() == 'y') // wait for confirmation of the user
    {
        do
        {
            snakeGame NewSnake(level++);
            checked = NewSnake.PlayGame();
        } while (AskUserToPlayContinue() == 'y');
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
    ClearCentre(3, 2.5);
    printw("Welcome to the Snake Game. Are you ready? (y/n)");
    return UserInput();
}

// print end of the game menu and ask user to play again
int AskUserToPlayAgain()
{
    ClearCentre(3, 2.5);
    printw("Do you want to play again? (y/n)");
    return UserInput();
}

int AskUserToPlayContinue()
{
    ClearCentre(3, 2.5);
    printw("Mission Complete Do you want to play Continue? (y/n)");
    return UserInput();
}