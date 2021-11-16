//
// Created by hburn7 on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_INPUTHANDLER_H
#define OTHELLOPROJECT_CPP_INPUTHANDLER_H

#include <algorithm>
#include <iostream>
#include <string.h>

#include "../../Utils.h"
#include "../Directive.h"
#include "../../OthelloGame/Color.h"

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
     * @param color The color of the player (or agent) we are looking at.
     * @return Matching Directive
     */
    static Directive identifyDirective(std::string input, int color);
};

#endif //OTHELLOPROJECT_CPP_INPUTHANDLER_H
