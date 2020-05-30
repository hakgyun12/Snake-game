#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib> //scrand함수 사용
#include <ctime> //time함수 사용

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

using namespace std;
//정의 집계 데이터 유형(그룹화) -> 하나 이상변수를 그룹지어 새로운 자료형 정의
struct CharPosition //CharPosition이라는 자료형 정의
{
    int x, y;
    CharPosition(int col, int row);
    CharPosition();
};

class snakeGame
{
private:
    int del, maxwidth, maxheight, speedChange, FruitTimer, PoisonTimer; 
    char direction, partchar, edgechar, fruitchar, poisonchar;
    // partchar 뱀을 나타내는 문양
    // edgechar window의 edge
    // fruitchar 성장 아이템 문양
    // poisonchar 감소 아이템 문양
    // del 속도
    //점수판 표시 변수
    int scoreGrowthItem, scorePoisonItem, scoreGate, currentLength, maxLength;
    bool bEatsFruit;
    bool bEatsPoison;
    CharPosition fruit;              //성장아이템
    CharPosition poison;            //감소아이템
    vector<CharPosition> snake; // 뱀 몸

    //window생성
    void InitGameWindow();
    //벽생성
    void DrawWindow();
    // 뱀의 모양 구현
    void DrawSnake();
    // 현재 몸의길이,먹은아이템갯수 표시
    void Print();
    //성장 아이템 추가 위치 함수
    void PositionFruit();
    //감소 아이템 추가 위치 함수
    void PositionPoison();
    // 패배하는 조건
    bool FatalCollision();
    // 뱀의 움직임 구현
    void MoveSnake();
    //성장 아이템을 먹었을 경우
    bool GetsFruit();
    //감소 아이템을 먹었을 경우
    bool GetsPoison();
    //먹지않았을때 위치 변겅
    void FruitTime();
    void PoisonTime();

public:
    snakeGame();
    ~snakeGame(); // 메모리할당 삭제
    void PlayGame();
};

#endif