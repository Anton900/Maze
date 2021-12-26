#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "maze.h"

bool validNumber(std::string s);
void helpText();
void errorCheck(int size, int &rows, int &cols, bool check, std::string input, std::string output);
void missingArguments(int cols, int rows);
void inputOption(bool check, std::string input);
void createMazeOption(int cols, int rows, std::string output);

int main(int argc, char **argv)
{
    int c;
    int cols = 0;
    int rows = 0;
    int size = 0;
    bool check = false;
    std::string input = "";
    std::string output = "";
    try
    {

        while (1)
        {
            static struct option long_options[] =
                {
                    {"version", no_argument, 0, 'v'},
                    {"help", no_argument, 0, 'h'},
                    {"size", required_argument, 0, 's'},
                    {"columns", required_argument, 0, 'c'},
                    {"rows", required_argument, 0, 'r'},
                    {"input", required_argument, 0, 'i'},
                    {"output", required_argument, 0, 'o'},
                    {"check", no_argument, 0, 'b'},
                    {0, 0, 0, 0}};
            int option_index = 0;
            c = getopt_long(argc, argv, "vhs:c:r:i:o:b",
                            long_options, &option_index);

            if (c == -1)
                break;
            switch (c)
            {

            case 'v':
            {
                std::cout << "version 1.0\n";
                return EXIT_SUCCESS;
            }

            case 'h':
            {
                helpText();
                return EXIT_SUCCESS;
            }

            case 's':
            {
                std::string option = optarg;
                if (!validNumber(option))
                    throw std::string("Not a valid number for size. It has to be an uneven number of atleast size 5.");
                size = stoi(option);
                break;
            }
            case 'c':
            {
                std::string option = optarg;
                if (!validNumber(option))
                    throw std::string("Not a valid number for columns. It has to be an uneven number of atleast size 5.");
                cols = stoi(option);
                break;
            }
            case 'r':
            {
                std::string option = optarg;
                if (!validNumber(option))
                    throw std::string("Not a valid number for rows. It has to be an uneven number of atleast size 5.");
                rows = stoi(option);
                break;
            }
            case 'i':
            {
                input = optarg;
                std::ifstream inFile(input);
                if (inFile.fail() || inFile.bad())
                {
                    std::cerr << "The file " << input << " doesn't exist or it's corrupt.\n";
                    return EXIT_FAILURE;
                }
                inFile.close();
                break;
            }
            case 'o':
            {
                output = optarg;
                break;
            }
            case 'b':
            {
                check = true;
                break;
            }

            case '?':
                return EXIT_FAILURE;

            default:
                return EXIT_SUCCESS;
            }
        }
        errorCheck(size, rows, cols, check, input, output); // If statements for error checking. Throws exception if error found
        if (input.size() > 0)                               // If input has a filename
        {
            inputOption(check, input);
            return EXIT_SUCCESS;
        }
        missingArguments(cols, rows); // Throws exception if missing arguments
        if (cols >= 5 && rows >= 5)   // If valid columns and rows
            createMazeOption(cols, rows, output);
        return EXIT_SUCCESS;
    }
    catch (std::string &e)
    {
        std::cerr << e << "\n";
    }
    catch (std::out_of_range &e)
    {
        std::cerr << e.what() << " exception: number is out of range. Try a smaller number.\n";
    }
    catch (std::bad_array_new_length &e)
    {
        std::cerr << e.what() << " exception: array is too large. Try a smaller number.\n";
    }
    catch (std::bad_alloc &e)
    {
        std::cerr << e.what() << " exception: failed to allocate enough memory for the maze. Try a smaller number.\n";
    }
    catch (...)
    {
        std::cerr << "Unknown error. Type --help for assistance.\n";
    }
    return EXIT_FAILURE;
}

/*
Checks if the string is a valid uneven interger for columns or rows.
@param value is string to be checked
@return true of false whether the string is valid or not
*/
bool validNumber(std::string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        if (!isdigit(s[i]))
            return false;
    }
    int n;
    try
    {
        n = stoi(s);
    }
    catch (std::out_of_range &e)
    {
        throw(e);
    }
    if (n % 2 == 0 || n < 5) // Check if even or lower then 5
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
    if ((size > 0 || (cols > 0 || rows > 0)) && (check || input.size() > 0)) // If attempting to both create a maze and check solution/solve maze.
        throw std::string("Conflicting commands, uncertain on what to do.\nUse --help if you need assistance.");
    if (size > 0 && (cols > 0 || rows > 0)) // -s command can't be included with -c or -r commands
        throw std::string("Can't use --rows and --columns at the same time as --size");
    else if (size > 0) // If only -s command is used
        cols = size, rows = size;
    if (input.size() > 0 && output.size() > 0)
        throw std::string("Can't have both input and output at the same time.");
    if (check && input.size() == 0)
        throw std::string("Missing --input command to check file.");
}

/*
Checks if columns or rows are missing arguments. Throws string if argument missing is found
@param values is cols and rows
*/
void missingArguments(int cols, int rows)
{
    if (cols == 0 && rows == 0)
        throw std::string("Missing commands. Use --help command if you need assistance.");
    else if (cols == 0)
        throw std::string("Missing number for columns. Use the --help command if you need assistance.");
    else if (rows == 0)
        throw std::string("Missing number for rows. Use the --help command if you need assistance.");
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
    if (check && maze.solutionExists())
        std::cout << "Solution found.\n";
    else if (!maze.solutionExists())
        throw std::string("No solution found");
    else if (maze.solutionExists())
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
    if (output.size() > 0)
    {
        std::ofstream outFile(output);
        if (outFile.fail() || outFile.bad())
            throw std::string("Couldn't create file.");
        outFile << maze;
        std::cout << "Saved maze in " << output << "\n";
        outFile.close();
    }
    else
        maze.display();
}