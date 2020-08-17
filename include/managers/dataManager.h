#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Local dependencies
#include <handlers/fileHandler.h>
#include <const.h>

// Write CSV table 'text' to file 'ṕath'
short write_table(const std::string path, const std::string text);

#endif