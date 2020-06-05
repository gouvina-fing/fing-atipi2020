// Local dependencies
#include <handlers/errorHandler.h>

// Print error message corresponding to error code 'code'
void error_msg(const unsigned short code) {

    if (code == ERROR_MAIN_BAD_FORMATTING) std::cout << ERROR_MAIN_BAD_FORMATTING_MSG;

    if (code == ERROR_FILE_UNKNOWN) std::cout << ERROR_FILE_UNKNOWN_MSG;
    if (code == ERROR_FILE_DAMAGED) std::cout << ERROR_FILE_DAMAGED_MSG;
    if (code == ERROR_FILE_BAD_HEADER) std::cout << ERROR_FILE_BAD_HEADER_MSG;
    if (code == ERROR_FILE_BAD_DIMENSIONS) std::cout << ERROR_FILE_BAD_DIMENSIONS_MSG;
    if (code == ERROR_FILE_BAD_BPP) std::cout << ERROR_FILE_BAD_BPP_MSG;

}