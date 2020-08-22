#ifndef LOG_HANDLER_H
#define LOG_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// Local dependencies
#include <const.h>

// Print log message 'msg'
void log_msg(const std::string msg);

// Print error message corresponding to error code 'code'
void error_msg(const unsigned short code);

// Print run message based on option 'op'
void run_msg(const unsigned short op);

#endif