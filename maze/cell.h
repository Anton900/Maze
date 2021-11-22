#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <map>
#include <set>
#include <list>
#include <time.h>

class Cell
{
    public:
        char state;
        bool borderWall = false;
        bool visited = false;
        int row;
        int col;
    
    public:
        char getState();
        void setVisited();
        void setBorderWall();
        void setState(char c);
        void setColumn(int i);
        void setRow(int i);

        Cell& operator=(const Cell& rhs);

};

#endif