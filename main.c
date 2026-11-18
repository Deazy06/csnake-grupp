#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

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


#define clrscr() printf("\e[1;1H\e[2J")
void gotoxy(int x,int y){
    printf("%c[%d;%df",0x1B,y,x);
}



void drawBoundaries(){
    for(int row = 0; row < ROWS+2;row++){
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


void drawSnake(Snake snake){
    gotoxy(snake.X,snake.Y);
    printf("@");
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

int main(){
    Snake body [100];
    int length = 6;
    body[0].X = 10; 
    body[0].Y = 10;

    for(int i = 1; i < length; i++){
        body[i].X = body[i-1].X - 1;
        body[i].Y = body[i-1].Y;
    }

    clrscr();
    hideCursor();
    drawBoundaries(1);

    KeyboardDir direction = KeyboardDir_Right; // store the current direction of the snake

    while(1){
        //clrscr();
        //drawBoundaries(1);
        clearSnake(body[0]);
        gotoxy(0,0);
        char ch = getNextKeyboardAction();
        if(ch == 'w') direction = KeyboardDir_Up; // update the direction based on user input
        else if(ch == 's') direction = KeyboardDir_Down;
        else if(ch == 'a') direction = KeyboardDir_Left;
        else if(ch == 'd') direction = KeyboardDir_Right;
        moveSnake(&body[0], direction); // move the snake in the current direction

        for(int i = 1; i < length; i++){
            drawSnake(body[i]);
        }

        drawSnake(body[0]);
        msleep(250);
    }
    return 1;
}

