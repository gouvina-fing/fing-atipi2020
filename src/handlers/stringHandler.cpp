// Local dependencies
#include <handlers/stringHandler.h>

// Get base text from specific formatted text
std::string parse_base(const std::string text) {
    size_t lastS = text.find_last_of("S");
    return text.substr(0, lastS); 
}

// Get base text from specific formatted text removing file extension
std::string parse_extension(const std::string text) {
    size_t lastdot = text.find_last_of(".");
    return text.substr(0, lastdot); 
}

// Get delta coef from specific formatted text
float parse_delta(const std::string text) {
    size_t lastdot = text.find_last_of(".");
    size_t lastS = text.find_last_of("S");
    return stof(text.substr(lastS + 1, lastdot)); 
}

// Get PSNR text from specific formatted text
std::string parse_PSNR(const std::string text) {
    std::stringstream test(text);
    std::string segment;

    std::vector<std::string> seglist;
    std::vector<std::string>::iterator it;

    while(std::getline(test, segment, '=')) {
        it = seglist.begin();
        seglist.insert(it, segment);
    }

    std::string result = seglist.front();
    return result.substr(1, result.length() - 4);
}