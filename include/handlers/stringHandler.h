#ifndef STRING_HANDLER_H
#define STRING_HANDLER_H

// Global dependencies
#include <stdlib.h>
#include <sstream>
#include <string>
#include <vector>

// Local dependencies
#include <const.h>

// Get base text from specific formatted text
std::string parse_base(const std::string text);

// Get base text from specific formatted text removing file extension
std::string parse_extension(const std::string text);

// Get delta coef from specific formatted text
float parse_delta(const std::string text);

// Get PSNR text from specific formatted text
std::string parse_PSNR(const std::string text);

#endif