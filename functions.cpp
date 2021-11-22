#include "functions.h"

/*
Checks if the string is a valid uneven interger for columns or rows.
@param value is string to be checked
@return true of false whether the string is valid or not
*/
bool validNumber(std::string s)
{
    for(int i = 0; i < s.size(); i++)
    {
        if(!isdigit(s[i])) return false;
    }
    int n;
    try
    {
        n = stoi(s);
    }
    catch(std::out_of_range &e) {throw (e);}
    if(n % 2 == 0 || n < 5) // Check if even or lower then 5
        return false;
    return true;
}

/*
Prints help text for the available commands
*/
void helpText()
{
    std::cout << "A list of all the available commands and arguments.\n\n"
              << "(-v, --version).\n\tPrint version number.\n\n"
              << "(-h, --help).\n\tPrint help text of all the available commands and arguments.\n\n"
              << "(-s, --size) N.\n\tCreate a labyrinth with size N. Requires argument.\n\n"
              << "(-c, --columns) W.\n\tCreate a labyrinth with the width N. Requires argument.\n\tUse with the --rows argument.\n\n"
              << "(-r, --rows) H.\n\tCreate a labyrinth with the height N. Requires argument.\n\tUse with the --columns argument.\n\n"
              << "(-i, --input) file.\n\tSolve a maze from a file. Requires argument.\n\n"
              << "(-o, --output) file.\n\tStore maze in a file. Requires argument.\n\n"
              << "(-b, --check).\n\tCheck if solution exists for maze. Use with --input command.\n";
              
}

/*
Checks the commands for errors, if there'are conflicting commands. Throws string if error found 
@param values are the commands to be checked
*/
void errorCheck(int size, int &rows, int &cols, bool check, std::string input, std::string output)
{
    if((size > 0 || (cols > 0 || rows > 0)) && (check || input.size() > 0)) // If attempting to both create a maze and check solution/solve maze.
        throw std::string("Conflicting commands, uncertain on what to do.\nUse --help if you need assistance.");
    if(size > 0 && (cols > 0 || rows > 0)) // -s command can't be included with -c or -r commands
        throw std::string("Can't use --rows and --columns at the same time as --size");
    else if(size > 0) // If only -s command is used
        cols = size, rows = size;
    if(input.size() > 0 && output.size() > 0)
        throw std::string("Can't have both input and output at the same time.");
    if(check && input.size() == 0) 
        throw std::string("Missing --input command to check file.");
}

/*
Checks if columns or rows are missing arguments. Throws string if argument missing is found
@param values is cols and rows
*/
void missingArguments(int cols, int rows)
{
    if(cols == 0 && rows == 0)
        throw std::string("Missing commands. Use --help command if you need assistance.");
    else if(cols == 0)
        throw std::string ("Missing number for columns. Use the --help command if you need assistance.");
    else if(rows == 0)
        throw std::string ("Missing number for rows. Use the --help command if you need assistance.");
}

/*
The function for the --input and --check command.
Prints the solution to the maze with the --input command.
Prints solution found if used with the --check command and solution is possible.
Print solution not found if there's no solution.
@param values are check for checking for solution if true and string input for filename
*/
void inputOption(bool check, std::string input)
{
    Maze maze;
    maze.readMaze(input);
    maze.solveMaze();
    if(check && maze.solutionExists())
        std::cout << "Solution found.\n";
    else if(!maze.solutionExists())
        throw std::string("No solution found");
    else if(maze.solutionExists())
        maze.display();
}

/*
The function to create a maze and write to terminal or file.
Creates a maze with the value of cols and rows.
Writes to file if output is not empty, otherwise writes to terminal with cout.
@param values are the columns and rows for maze and output is filename to write to if not empty string.
*/
void createMazeOption(int cols, int rows, std::string output)
{
    Maze maze(rows, cols);
    maze.generateMaze();
    if(output.size() > 0)
    {
        std::ofstream outFile(output);
        if(outFile.fail() || outFile.bad())   
                throw std::string ("Couldn't create file.");
        outFile << maze;
        std::cout << "Saved maze in " << output << "\n";
        outFile.close();
    }
    else
        maze.display();
}