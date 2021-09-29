//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
#define OTHELLOPROJECT_CPP_OUTPUTHANDLER_H

#include "../Directive.h"
#include "../../OthelloGame/OthelloColor.h"
#include "../../Utils.h"
#include <string.h>
#include <array>

class OutputHandler {
public:
    /**
     * Sends referee-compatible output to stdout. This method is only run after
     * whichever d passed to this method has been successfully parsed and
     * all relevant operations have been completed.
     * @param d Instruction that was just confirmed.
     * @param input Original input from console / opponent (bot).
     */
    static void outputDirective(Directive d, const std::string &input);
    /**
     * Returns a move set for a given (valid) input. Returns an invalid array { -1, -1 } if the input is invalid.
     * @param input
     * @return
     */
    static int toPos(std::string input);
    static std::string getMoveOutput(OthelloColor c, int pos);
};


#endif //OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
