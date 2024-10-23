//Run in dev c++

#include<stdio.h>  
#include<conio.h>  
#include<graphics.h>  
#include<dos.h> 
#include<iostream>
using namespace std;
int main()  
{     
    initwindow(1530,800,"Navigation system");
    setcolor(YELLOW);
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,5);
    outtextxy(420,30,"CEG NAVIGATION SYSTEM");
    readimagefile("map.jpeg",100,100,1400,700);
    getch();
    closegraph();
}