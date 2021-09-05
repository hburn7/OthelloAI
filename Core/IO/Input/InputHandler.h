//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_INPUTHANDLER_H
#define OTHELLOPROJECT_CPP_INPUTHANDLER_H

#include <string>

#include "../Directive.h"
#include "../../OthelloGame/OthelloColor.h"

class InputHandler {
public:
    /**
     * Reads input from stdin.
     * @return Console input
     */
    static std::string readInput();
    /**
     * Identifies which directive is given from the console input.
     * @param input Console input
     * @return Matching Directive
     */
    static Directive identifyDirective(std::string input);
    /**
     * Othello board is in lettered columns, a-h. This will convert the character to the desired index.
     * @param column
     * @return
     */
    static int charToInt(char column);

private:
    static int parseInt(const std::string &input);
};

#endif //OTHELLOPROJECT_CPP_INPUTHANDLER_H
