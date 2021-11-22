#ifndef MAZE_H
#define MAZE_H

#include "cell.h"

class Maze
{
    private:
        const char WALL = '*';
        const char PATH = ' ';
        const char TRAV = 'x';
        int rows;
        int cols;
        Cell **maze = nullptr;
        std::stack <Cell> visited;

    public:
        Maze();
        Maze(int rows, int cols);
        ~Maze();

        void fillMaze();
        void generateMaze();
        void saveMaze();
        void readMaze(std::string file);
        void solveMaze();
        bool validMaze(std::string file);
        void display();
        void setRows(int rows);
        void setCols(int cols);
        int getRows(){return rows;};
        int getCols(){return cols;};
        bool empty();
        bool solutionExists();
        void randomExit();
        std::vector <Cell> getNeighbors(int y, int x);
        std::vector <Cell> findPaths(int y, int x);
        friend std::ostream& operator<< (std::ostream& os, Maze& maze);

};

#endif