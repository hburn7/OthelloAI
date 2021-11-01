//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
#define OTHELLOPROJECT_CPP_OUTPUTHANDLER_H

#include <array>
#include <iostream>
#include <string.h>

#include "../Directive.h"
#include "../../OthelloGame/Color.h"
#include "../../OthelloGame/Move.h"
#include "../../Utils.h"
#include "../../Logger.h"

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
     * Returns a move for a given (valid) input. Returns a pass if the input is invalid.
     * @param input
     * @return
     */
    static Move toMove(std::string input);
    /**
     * Returns a referee-compatible std::string representing a move for the given color and position on the board.
     * @param c The color of the player making the move
     * @param move The move we are moving to.
     * @param logComment Whether to also log a debug comment about the position to string conversion
     * @return A referee-compatible std::string representing this move.
     */
    static std::string getMoveOutput(int c, Move move, bool logComment);
};


#endif //OTHELLOPROJECT_CPP_OUTPUTHANDLER_H
