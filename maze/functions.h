#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "maze.h"

bool validNumber(std::string s);
void helpText();
void errorCheck(int size, int &rows, int &cols, bool check, std::string input, std::string output);
void inputOption(bool check, std::string input);
void createMazeOption(int cols, int rows, std::string output);
void missingArguments(int cols, int rows);

#endif