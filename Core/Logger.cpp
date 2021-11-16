//
// Created by hburn7 on 8/29/21.
//

#include "Logger.h"

void Logger::logComment(const std::string &s, bool newLine) {
    if(newLine) {
        std::cout << "C " << s << std::endl;
    } else {
        std::cout << s;
    }
}
