#include "maze.h"

Maze::Maze()
{
    rows = 0;
    cols = 0;
}

Maze::Maze(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    fillMaze(); // Fill maze with walls
}

/*
Creates a matrix for the maze with all cells as walls
*/
void Maze::fillMaze()
{
    maze = new Cell *[rows];
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
            maze[i] = new Cell [cols];
    }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(i == 0 || i == rows - 1 || j == 0 || j == cols - 1)
                maze[i][j].setBorderWall(); // Set the outer cells as borderwalls
            maze[i][j].setRow(i); // Set cell row coordinates
            maze[i][j].setColumn(j); // Set cell column coordinates
            maze[i][j].setState(WALL); // Set cell to wall
        }
    }
}

/*
Free up memory when Maze object goes out of scope
*/
Maze::~Maze()
{
    for(int i = 0; i < rows; i++)
        delete[] maze[i];
    delete[] maze;
}

/*
Generate a randomized maze with the DFS algorithm.
*/
void Maze::generateMaze()
{
    if(empty()) // If empty, fill maze with walls
        fillMaze();
    srand(time(NULL));
    maze[1][0].setState('S'); // Mark start of maze
    maze[1][1].setState(PATH); // Open path for starting cell
    maze[1][1].setVisited(); 
    visited.push(maze[1][1]); // Push in first cell
    int y = 1, x = 1; // Starting coordinates
    std::vector <Cell> neighbors;
    while(!visited.empty()) // As long as all cells have been visited
    {
        neighbors = getNeighbors(y, x);
        if(neighbors.empty() && !visited.empty())
        {
            Cell next = visited.top(); 
            visited.pop(); // Remove cell if it has no neighbors left
            y = next.row; // Go to next cell in stack
            x = next.col;
        }
        if(neighbors.size() > 1) // Push in cell if it has more then one neighbor
            visited.push(maze[y][x]);
        if(neighbors.size() > 0)
        {
            int random = rand() % neighbors.size();
            Cell next = neighbors[random]; // Go to a random neighbor of current cell
            if(next.row > y) y += 1;
            else if(next.row < y) y -= 1;
            if(next.col > x) x += 1;
            else if(next.col < x) x -= 1;
            maze[y][x].setState(PATH); // Remove inbetween wall of current cell and neighbor cell
            y = next.row; // Go to neighbor cell's coordinates
            x = next.col;
            maze[y][x].setVisited(); //mark neighbor cell has visited
            maze[y][x].setState(PATH); //make path
        }
        neighbors.clear();
    }
    randomExit(); // Set the exit at a random border wall
}

/*
Mark a random border wall as the exit.
It has to be an border wall on either the last row or the last column.
*/
void Maze::randomExit()
{
    int random = rand() % 2;
    if(random) // If 1, mark a random borderwall on the last row
    {
        random = (rand() % (cols - 2)) + 2;
        while(maze[rows - 2][cols - random].state == '*') // The end can't be a wall, search another row
            random = (rand() % (cols - 2)) + 2;
        maze[rows - 1][cols - random].state = 'E'; // Remark borderwall to exit
    }
    else // If 0, mark a random borderwall on the last column
    {
        random = (rand() % (rows - 2)) + 2;
        while(maze[rows - random][cols - 2].state == '*') // The end can't be a wall, search another column
            random = (rand() % (cols - 2)) + 2;
        maze[rows - random][cols - 1].state = 'E'; // Remark borderwall to exit
    }
}

/*
Function to read a maze from file. If invalid data for maze it throws a exception.
@param value is the filename to be read from.
*/
void Maze::readMaze(std::string file)
{
    std::ifstream inFile(file);
    if(inFile.fail())
    {
        std::cout << "\nThe file " << file << " couldn't open. It might not exist or it's corrupt.\n";
        return;
    }
    std::string line;
    int numLines = 0;
    while(!inFile.eof())
    {
        std::getline(inFile, line);
        if(line.length() % 2 == 0 || line.length() < 5)
            throw std::string("The file has invalid number of columns. It has to be an uneven number of atleast size 5.");
        numLines++;
    }
    inFile.close();
    cols = line.length();
    rows = numLines;
    if(rows % 2 == 0 || rows < 5)
    {
        rows = 0;
        throw std::string("The file has invalid number of rows. It has to be an uneven number of atleast size 5.");
    }
    maze = new Cell *[rows]; // Create matrix for maze
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
            maze[i] = new Cell [cols];
    }
    inFile.open(file);
    std::getline(inFile, line);
    for(int i = 0; i < rows; i++) // Give the maze cells coordinates and char state
    {
        for(int j = 0; j < cols; j++)
        {
            if(line[j] != PATH && line[j] != WALL && line[j] != 'S' && line[j] != 'E')
                throw std::string("Incorrect data for maze found in input file");
            maze[i][j].setState(line[j]);
            maze[i][j].setRow(i);
            maze[i][j].setColumn(j);
        }
        getline(inFile, line);
    }
    inFile.close();
}

/*
Get all the valid neighbor for a cell. It checks right, left, up and down. The neighbor cell can't be a borderwall or been visited before.
@param values are the cell coordinates to check from
@return a vector with all found neighbor cells
*/
std::vector <Cell> Maze::getNeighbors(int y, int x)
{
    std::vector<Cell> vec;
    if(!maze[y][x+1].borderWall && maze[y][x+2].visited == false) // Check right neighbor
        vec.push_back(maze[y][x+2]);
    if(!maze[y][x-1].borderWall && maze[y][x-2].visited == false) // Check left neighbor
        vec.push_back(maze[y][x-2]);
    if(!maze[y+1][x].borderWall && maze[y+2][x].visited == false) // Check up neighbor
        vec.push_back(maze[y+2][x]);
    if(!maze[y-1][x].borderWall && maze[y-2][x].visited == false) // Check down neighbor
        vec.push_back(maze[y-2][x]);
    return vec; // Return all the found neighbors
}

/*
Get all the valid paths for a cell. It checks right, left, up and down. The neighbor cell must be a path and unvisited.
@param values are the cell coordinates to check from
@return a vector with all found paths
*/
std::vector <Cell> Maze::findPaths(int y, int x)
{
    std::vector <Cell> vec;
    if(maze[y][x+1].state == PATH && maze[y][x+1].visited == false) // Check path to the right
       vec.push_back(maze[y][x+1]);
    if(maze[y][x-1].state == PATH && maze[y][x-1].visited == false) // Check path to the left
        vec.push_back(maze[y][x-1]);
    if(maze[y+1][x].state == PATH && maze[y+1][x].visited == false) // Check path up
        vec.push_back(maze[y+1][x]);
    if(maze[y-1][x].state == PATH && maze[y-1][x].visited == false) // Check path down
        vec.push_back(maze[y-1][x]);
    return vec; // Return all the paths found
}

/*
Solve a maze by marking every path found with a 'x'. If the path leads to a dead end then remove the 'x' from the path til a new path can be searched.
Searches until found exit border wall or no more paths to search (no solution possible).
*/
void Maze::solveMaze()
{
    srand(time(NULL));
    maze[1][1].setState(TRAV); // Mark starting cell has travelled
    maze[1][1].setVisited(); // Mark starting cell has visited
    visited.push(maze[1][1]);
    std::vector <Cell> paths;
    int y = 1, x = 1;
    while((maze[y][x+1].state != 'E' && maze[y+1][x].state != 'E') && !visited.empty()) // As long not reached the end goal and solution is possible
    {
        paths = findPaths(y, x);
        while(paths.size() == 0 && !visited.empty()) // If cell has no paths then check the previous cell
        {
            visited.pop(); // Remove cell from stack if it has no paths left to give
            if(visited.empty()) // If there's no more cells then there's no solution
                break;
            maze[y][x].setState(PATH); // Remark the cell has a path because it leads to a dead end
            y = visited.top().row; // Go to next cell in stack
            x = visited.top().col;
            paths = findPaths(y, x);
        }
        if(paths.size() > 0)
        {
            int random = rand() % paths.size();
            Cell next = paths[random]; // Go to a random neighbor
            y = next.row; // Set coordinates for neighbor
            x = next.col;
            maze[y][x].setState(TRAV); // Mark neighbor as traveled
            maze[y][x].setVisited(); // Mark neighbor as visited
            visited.push(maze[y][x]);
        }
        paths.clear(); 
    }
}

/*
Check if maze is empty by checking where it's pointing.
@return true or false whether maze is empty
*/
bool Maze::empty()
{
    return maze == nullptr;
}

/*
Set the rows variable for maze.
@param value is the value to be set from.
*/
void Maze::setRows(int rows)
{
    this->rows = rows;
}

/*
Set the cols variable for maze.
@param value is the value to be set from.
*/
void Maze::setCols(int cols)
{
    this->cols = cols;
}

/*
Check if solution is possible by whether the stack of visited cells is empty or not
@return true or false whether the stack is empty
*/
bool Maze::solutionExists()
{
    if(visited.empty())
        return false;
    return true;
}

/*
Print out maze.
*/
void Maze::display()
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            std::cout << maze[i][j].getState();
        }
        std::cout << "\n";
    }
}

/*
Ostream operators '<<' on how to write Maze to file.
@param values are ostream and Maze object
@return the ostream object
*/
std::ostream& operator << (std::ostream& os, Maze& maze)
{
    for(int i = 0; i < maze.getRows(); i++)
    {
        for(int j = 0; j < maze.getCols(); j++)
        {
            os << maze.maze[i][j].getState();
        }
        if(i != maze.getRows() - 1)
            os << "\n";
    }
    return os;
}