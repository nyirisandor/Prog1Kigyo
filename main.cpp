#include <iostream>
#include <string>
#include "Matrix.h"
#include "kigyosjatek.h"


#define APPLESYMBOL 'A'
#define CHEWINGGUMSYMBOL 'X'

#define APPLECOUNT 20
#define CHEWINGGUMCOUNT 3
#define CHEWINGGUMDELAY 3

#define ROWS 5
#define COLS 5

int main(){
    kigyosjatek jatek = kigyosjatek(kigyosmap(ROWS,COLS,coords(0,0),APPLECOUNT,CHEWINGGUMCOUNT,APPLESYMBOL,CHEWINGGUMSYMBOL),CHEWINGGUMDELAY);

    jatek.printMap();

    while (!jatek.checkForWin())
    {
        jatek.update();
    }
    


    return 0;
}