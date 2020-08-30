// Local dependencies
#include <handlers/fileHandler.h>

// Check if file 'path' exists
short check_file(const std::string path) { 
    std::ifstream file(path);
    if (file.good()) return OK;
    return ERROR_FILE_UNKNOWN;
}

// Get file name as string from 'path'
std::string get_file_name(const std::string path) { 
    
    std::stringstream test(path);
    std::string segment;

    std::vector<std::string> seglist;
    std::vector<std::string>::iterator it;

    while(std::getline(test, segment, '/')) {
        it = seglist.begin();
        seglist.insert(it, segment);
    }

    return seglist.front();
}

// Create directory specified in 'path'
short add_directory(const std::string path) {
    if (std::filesystem::create_directory(path))
        return OK;
    else
        return ERROR_DIRECTORY;
}

// Remove directory specified in 'path'
short remove_directory(const std::string path) {
    if (std::filesystem::remove_all(path))
        return OK;
    else
        return ERROR_DIRECTORY;
}

// Create and write in file 'path' a CSV table contained in text
short write_CSV(const std::string path, const std::string text) {

    std::ofstream file(path, std::ios_base::app);
    if (file.is_open()) {
        file << text << std::endl;
    } else {
        file.close();
        return ERROR_FILE_DAMAGED;
    }

    file.close();
    return OK;
}
