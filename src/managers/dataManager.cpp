// Local dependencies
#include <managers/dataManager.h>

// Write CSV table 'text' to file 'ṕath'
short write_table(const std::string path, const std::string text) {

    // Aux variables
    short code = OK;

    // Write CSV
    code = write_CSV(path,text);
    if (code != OK) {
        return code;
    }

    return OK;
}