#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Local dependencies
#include <const.h>

// Print error message corresponding to error code 'code'
void error_msg(const unsigned short code);

#endif