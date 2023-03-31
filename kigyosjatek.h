#include "Matrix.h"

#ifdef WIN32
    #include <conio.h>
    #define KEY_UP 72
    #define KEY_DOWN 80
    #define KEY_LEFT 75
    #define KEY_RIGHT 77
#else
   #include <ncurses.h> 
#endif

#pragma once


struct coords{
    int col,row;

    coords(){
        this->col = 0;
        this->row = 0;
    }

    coords(int col, int row){
        this->col = col;
        this->row = row;
    }
};

class kigyosMatrix : public Matrix<char>{
    public:
    kigyosMatrix(){}

    kigyosMatrix(int cols, int rows, char defaultValue = 0){
        this->dataArray = new char[rows*cols];
        this-> colCount = cols;
        this-> rowCount = rows;
        for (size_t i = 0; i < cols*rows; i++)
        {
            this->dataArray[i] = defaultValue;
        }
    }

    ~kigyosMatrix(){
        delete[] this->dataArray;
    }


    char& at(int i, int j){
        if(i >= this->colCount || j >= this->rowCount){
            throw std::out_of_range("Out of range");
        }

        return(this->dataArray[i*(colCount)+j]);
    }

    char& at(coords pos){
        if(pos.col >= this->colCount || pos.row >= this->rowCount){
            throw std::out_of_range("Out of range");
        }

        return(this->dataArray[pos.row*(colCount)+pos.col]);
    }
};

enum direction{
    up,
    down,
    left,
    right
};

class kigyosmap{
    private:
        kigyosMatrix* map;
        coords playerPosition;

        int appleCount;
        int chewinggumCount;

        char appleSymbol;
        char chewinggumSymbol;
        char grassSymbol;
    public:

        kigyosmap(){};

        kigyosmap(int rows, int cols, coords playerPosition = coords(0,0), int appleCount = 5, int chewinggumCount = 3,  char appleSymbol = 'A', char chewinggumSymbol = 'X',char grassSymbol = '.'){
            this->map = new kigyosMatrix(cols,rows,grassSymbol);
            this->playerPosition = playerPosition;

            this->appleSymbol = appleSymbol;
            this->chewinggumSymbol = chewinggumSymbol;
            this->grassSymbol = grassSymbol;

            this->generateMap(appleCount,chewinggumCount);
        }

        void generateMap(int appleCount, int chewinggumCount){
            if(appleCount + chewinggumCount + 1 > this->map->Cols()*this->map->Rows()){
                throw std::out_of_range("Too many apples and chewing gums on map");
            }

            this->appleCount = 0;
            this->chewinggumCount = 0;

            srand(time(NULL));

            this->map->reset(grassSymbol);

            this->map->at(this->playerPosition) = 'P';

            while (this->appleCount < appleCount)
            {
                coords randomcoords = coords(rand()%this->map->Cols(),rand()%this->map->Rows());
                if(this->map->at(randomcoords) == this->grassSymbol){
                   this->map->at(randomcoords) = appleSymbol;
                   this->appleCount++; 
                }
            }

            while (this->chewinggumCount < chewinggumCount)
            {
                coords randomcoords = coords(rand()%this->map->Cols(),rand()%this->map->Rows());
                if(this->map->at(randomcoords) == this->grassSymbol){
                   this->map->at(randomcoords) = chewinggumSymbol;
                   this->chewinggumCount++; 
                }
            }
        }

        void print(char separator = ' '){
            #ifdef WIN32
                for (size_t i = 0; i < this->map->Rows(); i++)
                {
                    for (size_t j = 0; j < this->map->Cols(); j++)
                    {
                        std::cout << this->map->at(i,j) << separator;
                    }
                    std::cout << std::endl;
                }
            #else
                for (size_t i = 0; i < this->map->Rows(); i++)
                {
                    for (size_t j = 0; j < this->map->Cols(); j++)
                    {
                        std::string a = "";
                        a += this->map->at(i,j);
                        a += separator;
                        printw(a.c_str());
                    }
                    printw("\n");
                }
            #endif
            
        }

        int getAppleCount(){
            return this->appleCount;
        }

        coords getPos(){
            return this->playerPosition;
        }

        char move(direction direction,int chewinggumDelay){
            coords newPos = this->playerPosition;

            switch (direction)
            {
            case 0:
                newPos.row--;
                break;
            case 1:
                newPos.row++;
                break;
            case 2:
                newPos.col--;
                break;
            case 3:
                newPos.col++;
                break;
            default:
                #ifdef WIN32
                    std::cout << "Invalid direction!" << std::endl;
                #endif
                return 0;
            }

            if(newPos.col >= this->map->Cols() || newPos.col < 0 || newPos.row >= this->map->Rows() || newPos.row < 0){
                #ifdef WIN32
                    std::cout << "Invalid position!" << std::endl;
                #endif
                return 0;
            }

            this->map->at(playerPosition) = this->grassSymbol;
            
            char pickedUp = this->map->at(newPos);

            this->map->at(newPos) = 'P';

            this->playerPosition = newPos;


            if(pickedUp == this->appleSymbol){
                this->appleCount--;
            }
            else if(pickedUp == this->chewinggumSymbol){
                return chewinggumDelay;
            }

            return 1;
        }
};


class kigyosjatek{
    private:
        kigyosmap map;

        int steps = 0;
        bool isPlaying = true;

        int chewinggumDelay;

    public:
        kigyosjatek(){};

        kigyosjatek(kigyosmap map = kigyosmap(5,5),int chewinggumDelay = 3){
            this->map = map;
            this->chewinggumDelay = chewinggumDelay;
        }

        bool checkForWin(){
            if(this->map.getAppleCount() == 0){
                #ifdef WIN32
                    std::cout << "\tWin!" << std::endl << "\tSteps taken: " << this->steps << std::endl;
                #else
                    clear();
                    printw(("Win! \n\tSteps taken: " + std::to_string(this->steps) + "\n").c_str());
                    refresh();
                #endif
                this->isPlaying = false;
                return true;
            }

            return false;
        }

        void move(direction direction){
            if(isPlaying){
                this->steps += this->map.move(direction,this->chewinggumDelay);
                //this->checkForWin();
            }

            this->printMap();
        }

        void printMap(){
            #ifdef WIN32
                std::cout << "Steps: " << this->steps << std::endl;
            #else
                clear();
                printw(("Steps taken: " + std::to_string(this->steps) + "\n").c_str());
            #endif
            this->map.print();
        }

        void update(){
            switch(getch()) {
            case KEY_UP:
                this->move(direction::up);
                break;
            case KEY_DOWN:
                this->move(direction::down);
                break;
            case KEY_LEFT:
                this->move(direction::left);
                break;
            case KEY_RIGHT:
                this->move(direction::right);
                break;
            default:
                break;
            }
        }
};