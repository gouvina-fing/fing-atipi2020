#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

// Local dependencies
#include <const.h>

// Execute command 'cmd' and retrieve its result as string
std::string exec(const char* cmd);

#endif