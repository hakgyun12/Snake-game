# Snake-game

C++ 프로그래밍 언어를 이용하여 Snake Game를 구현하도록 한다!



## 1. 사용자 메뉴얼

1. zip파일을 Download 받는다.

2. Terminal에 make run을 입력한다.

3. 게임화면을 나오면 y를 눌러 시작한다.

4. 게임을 진행하면서 4개의 Stage의 Mission을 Clear하도록 한다. 

5. (유의) 충돌하거나 Snake의 길이가 3미만이면 게임이 종료된다.

6. Stage 4까지 모든 Mission Clear 한다면 성공한다.



## 2. 실행 화면

<img width="365" alt="주석 2020-06-26 164713" src="https://user-images.githubusercontent.com/28584213/85833738-30047a80-b7cd-11ea-8b76-34ffaab23e32.png" style="zoom: 200%;" >

<img width="365" alt="주석 2020-06-26 164641" src="https://user-images.githubusercontent.com/28584213/85833755-3692f200-b7cd-11ea-9fd2-36eeeeb275a7.png" style="zoom: 200%;" >

<img width="365" alt="주석 2020-06-26 164617" src="https://user-images.githubusercontent.com/28584213/85833756-38f54c00-b7cd-11ea-993b-1050f11055b0.png" style="zoom: 200%;" >
<img width="367" alt="주석 2020-06-26 164810" src="https://user-images.githubusercontent.com/28584213/85833763-3abf0f80-b7cd-11ea-8462-acf2c125639b.png" style="zoom: 200%;" >
<img width="367" alt="주석 2020-06-26 164742" src="https://user-images.githubusercontent.com/28584213/85833776-3dba0000-b7cd-11ea-9d48-88c44fb3c14a.png" style="zoom: 200%;" >



## 3. Snake-game 단계별 구현

#### 1단계

- Ncurses Library 함수들을 사용하여 2차원 배열로 된 Snake Map을 Game 화면으로 표시한다.

#### 2단계

- 1단계 맵 위에 Snake를 표시하고, 화살표를 입력받아 Snake가 움직이도록 프로그램을 완성한다.

#### 3단계

- 2단계 프로그램에서, Map 위에 Growth Item와 Poison Item을 출현하도록 수정한다.
- 화면상에 표현 시, 색이나 기호를 달리하여 구분할 수 있도록 한다.

#### 4단계

- 3단계 프로그램에서, Map의 Wall안의 임의의 위치에 한 쌍의 Gate가 출현할 수 있도록 변경하고, 각 Gate에 Snake가 통과할 수 있도록 한다.

#### 5단계

- 4단계 프로그램에서, 우측에 게임 점수를 표시하는 화면을 구성한다.
- Mission을 달성하면 다음 Map으로 진행하도록 프로그램을 완성한다. (Stage는 최소 4개로 구성하고, 각 Stage의 Map은 서로 달라야한다.)

