#include <iostream>
#include <string>
#include "Matrix.h"
#include "kigyosjatek.h"

#ifndef WIN32
    #include <ncurses.h>
#endif


#define APPLESYMBOL 'A'
#define CHEWINGGUMSYMBOL 'X'

#define APPLECOUNT 20
#define CHEWINGGUMCOUNT 3
#define CHEWINGGUMDELAY 3

#define ROWS 5
#define COLS 5

int main(){
    #ifndef WIN32
        initscr();
        keypad(stdscr,true);
    #endif

    kigyosjatek jatek = kigyosjatek(kigyosmap(ROWS,COLS,coords(0,0),APPLECOUNT,CHEWINGGUMCOUNT,APPLESYMBOL,CHEWINGGUMSYMBOL),CHEWINGGUMDELAY);

    jatek.printMap();

    while (!jatek.checkForWin())
    {
        jatek.update();
    }
    


    return 0;
}