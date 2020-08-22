// Local dependencies
#include <handlers/logHandler.h>

// Print log message corresponding to log code 'code'
void log_msg(const std::string msg) {
    std::cout << msg << "\n";
}

// Print error message corresponding to error code 'code'
void error_msg(const unsigned short code) {

    if (code == ERROR_MAIN_BAD_FORMATTING) std::cout << ERROR_MAIN_BAD_FORMATTING_MSG;

    if (code == ERROR_FILE_UNKNOWN) std::cout << ERROR_FILE_UNKNOWN_MSG;
    if (code == ERROR_FILE_DAMAGED) std::cout << ERROR_FILE_DAMAGED_MSG;
    if (code == ERROR_FILE_BAD_HEADER) std::cout << ERROR_FILE_BAD_HEADER_MSG;
    if (code == ERROR_FILE_BAD_DIMENSIONS) std::cout << ERROR_FILE_BAD_DIMENSIONS_MSG;
    if (code == ERROR_FILE_BAD_BPP) std::cout << ERROR_FILE_BAD_BPP_MSG;
    if (code == ERROR_DIRECTORY) std::cout << ERROR_DIRECTORY_MSG;

}

// Print run message based on option 'op'
void run_msg(const unsigned short op) {

    std::string msg = "";

    if (op == 1)
        msg = "1: Basic DUDE over noisy images";
    else if (op == 2)
        msg = "2: Prefilter DUDE over noisy images";
    else return;

    std::cout << "Running option " <<  msg << "\n";
}
