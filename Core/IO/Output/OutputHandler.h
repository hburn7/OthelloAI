//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
#define OTHELLOPROJECT_CPP_OUTPUTHANDLER_H

#include "../Directive.h"
#include "../../OthelloGame/OthelloColor.h"
#include <string>
#include <array>

class OutputHandler {
public:
    /**
     * Sends referee-compatible output to stdout. This method is only run after
     * whichever directive passed to this method has been successfully parsed and
     * all relevant operations have been completed.
     * @param directive Instruction that was just confirmed.
     * @param input Original input from console / opponent (bot).
     */
    static void outputDirectiveConfirmation(Directive directive, const std::string &input);
    static void outputMove(OthelloColor color, std::array<int, 2> moveSet);
};


#endif //OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
