#include "cell.h"

/*
Copy constructor for cell
@param value is cell to be copied
@return the new cell
*/
Cell& Cell::operator=(const Cell& rhs)
{
    this->visited = rhs.visited;
    this->state = rhs.state;
    this->row = rhs.row;
    this->col = rhs.col;

    return *this;
}


void Cell::setVisited()
{
    visited = true;
}

char Cell::getState()
{
    return state;
}

void Cell::setState(char c)
{
    state = c;
}

void Cell::setBorderWall()
{
    borderWall = true;
}
void Cell::setColumn(int i)
{
    col = i;
}
void Cell::setRow(int i)
{
    row = i;
}