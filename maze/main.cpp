#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "functions.h"
#include "maze.h"

int main (int argc, char **argv)
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
                {0, 0, 0, 0}
            };
            int option_index = 0;
            c = getopt_long (argc, argv, "vhs:c:r:i:o:b",
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
                        if(!validNumber(option)) 
                            throw std::string("Not a valid number for size. It has to be an uneven number of atleast size 5.");
                        size = stoi(option);
                        break;
                    }
                case 'c':
                    {
                        std::string option = optarg;
                        if(!validNumber(option)) 
                            throw std::string("Not a valid number for columns. It has to be an uneven number of atleast size 5.");
                        cols = stoi(option);
                        break;
                    }
                case 'r':
                    {
                        std::string option = optarg;
                        if(!validNumber(option)) 
                            throw std::string("Not a valid number for rows. It has to be an uneven number of atleast size 5.");
                        rows = stoi(option);
                        break;
                    }
                case 'i':
                    {
                        input = optarg;
                        std::ifstream inFile(input);
                        if(inFile.fail() || inFile.bad())   
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
    if(input.size() > 0) // If input has a filename
    {
        inputOption(check, input);
        return EXIT_SUCCESS;
    }
    missingArguments(cols, rows); // Throws exception if missing arguments
    if(cols >= 5 && rows >= 5) // If valid columns and rows
        createMazeOption(cols, rows, output);
    return EXIT_SUCCESS;
    
    }
    catch(std::string &e) 
    {
        std::cerr << e << "\n";
    }
    catch(std::out_of_range &e) 
    {
        std::cerr << e.what() << " exception: number is out of range. Try a smaller number.\n";
    }
    catch(std::bad_array_new_length &e) 
    {
        std::cerr << e.what() << " exception: array is too large. Try a smaller number.\n";
    }
    catch(std::bad_alloc &e) 
    {
        std::cerr << e.what() << " exception: failed to allocate enough memory for the maze. Try a smaller number.\n";
    }
    catch(...)
    {
        std::cerr << "Unknown error. Type --help for assistance.\n";
    }
    return EXIT_FAILURE;
}