#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#include <time.h>
#include <errno.h>    

void hideCursor(){
    printf("\x1b[?25l");
}

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

#define ROWS 10 // 2-9
#define COLS 30 // 2-29
#define WALL '#'

typedef struct{
    int X;
    int Y;
}Snake;

typedef struct{
    int X;
    int Y;
}Food;


#define clrscr() printf("\e[1;1H\e[2J")

void gotoxy(int x,int y){
    printf("%c[%d;%df", 0x1B, y,x);
}

void foodxy(int x,int y){
    printf("%c[%d;%df",0x1B,y,x);
}



void drawBoundaries(){
    for(int row = 0; row < ROWS+2; row++){
        for(int col = 0; col < COLS+2; col++){
            if(row == 0 || row == ROWS+1 || col == 0 || col == COLS+1){
                printf("%c",WALL);         
            }
            else{
                printf(" ");
            }
        }
        printf("\n");
    }
}


void drawSnake(Snake snake, char c){
    gotoxy(snake.X,snake.Y);
    printf("%c",c);
}

void clearSnake(Snake snake){
    gotoxy(snake.X,snake.Y);
    printf(" ");
}


typedef enum {
    KeyboardDir_Left,    
    KeyboardDir_Right,
    KeyboardDir_Up,
    KeyboardDir_Down,
}KeyboardDir;


int kbhit2()
{
    return _kbhit();
}

int getNextKeyboardAction(){
    if(kbhit2()){
        //gotoxy(0,ROWS+3);
        //printf("Ange vad den ska göra:");
        char ch = getch();
        //printf("%d",ch);
        return ch;
    }
    return 0;
}



void moveSnake(Snake *snake,KeyboardDir direction){
    if(direction == KeyboardDir_Up){
        if(snake->Y == 2) snake->Y = ROWS+1;
        else snake->Y--;
    }
    if(direction == KeyboardDir_Down){
        if(snake->Y == ROWS+1) snake->Y = 2;
        else snake->Y++;
    }
    if(direction == KeyboardDir_Left){
        if(snake->X == 2) snake->X = COLS+1;
        else snake->X--;
    }
    if(direction == KeyboardDir_Right){
        if(snake->X == COLS+1) snake->X = 2;
        else snake->X++;
    }
}
int hitsWall(Snake snake, KeyboardDir direction){
    if(direction == KeyboardDir_Up    && snake.Y - 1 < 2)       return 1;
    if(direction == KeyboardDir_Down  && snake.Y + 1 > ROWS+1)  return 1;
    if(direction == KeyboardDir_Left  && snake.X - 1 < 2)       return 1;
    if(direction == KeyboardDir_Right && snake.X + 1 > COLS+1)  return 1;
    return 0;
}
int hitsSelf(Snake body[], int length, KeyboardDir direction){
    Snake next = body[0];
    moveSnake(&next, direction);

    for(int i = 1; i < length; i++){
        if(body[i].X == next.X && body[i].Y == next.Y) return 1;
    }
    return 0;
}
void random_pos(Food *food) {
    
    int minX = 2;
    int maxX = COLS + 1; 
    int minY = 2;
    int maxY = ROWS + 1;

    food->X = minX + rand() % (maxX - minX + 1);
    food->Y = minY + rand() % (maxY - minY + 1);
}

void drawfood(Food food){
    foodxy(food.X,food.Y);
    printf("*");
}

void clearfood(Food food){
    foodxy(food.X,food.Y);
    printf(" ");
}

bool eatfood (Snake snake, Food food) {
    if (food.X == snake.X && food.Y == snake.Y) {
    return true;
    } else {
        return false;
    }
}



int main(){
    srand(time(NULL));
    Food food;
    Snake body[100];
    int length = 1;
    int score = 0;
    body[0].X = 10;
    body[0].Y = 10;

    for(int i = 1; i < length; i++){
        body[i].X = body[i-1].X - 1;
        body[i].Y = body[i-1].Y;
    }

    clrscr();
    hideCursor();
    drawBoundaries();
    random_pos(&food);
    drawfood(food);

    gotoxy(1, ROWS+3);          // raden under spelplanen
    printf("Poang: %d", score);

    KeyboardDir direction = KeyboardDir_Right;

    while(1){
        clearSnake(body[length-1]); // sudda svansen
        gotoxy(0,0);
        char ch = getNextKeyboardAction();
        if(ch == 'w') direction = KeyboardDir_Up;
        else if(ch == 's') direction = KeyboardDir_Down;
        else if(ch == 'a') direction = KeyboardDir_Left;
        else if(ch == 'd') direction = KeyboardDir_Right;

        if (hitsWall(body[0], direction)) break; // collision wall detector
        if(hitsSelf(body, length, direction)) break; // collision body detector

        for (int i = length - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }
        
        moveSnake(&body[0], direction); // move the snake in the current direction

        for(int i = 1; i < length; i++){
            drawSnake(body[i], 'o');
        }
        drawSnake(body[0], 'O');

        if(eatfood(body[0], food)){
            if(length < 100){
                body[length] = body[length-1]; // ny del startar på svansen
                length++;
            }
            score += 10;
            gotoxy(1, ROWS+3);
            printf("Poang: %d", score);
            random_pos(&food);
            drawfood(food);
        }
        msleep(250);
    }
    gotoxy(1, ROWS+4);
    printf("Game over!\n");
    return 1;
}
