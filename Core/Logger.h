//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_LOGGER_H
#define OTHELLOPROJECT_CPP_LOGGER_H

#include <iostream>
#include <string.h>

class Logger {
public:
    /**
    * Logs a comment to standard output in format compatible with Othello referee.
    * @param s Text to log
    */
    static void logComment(const std::string &s, bool newLine = true);
};

#endif //OTHELLOPROJECT_CPP_LOGGER_H