//
// Created by Harry Burnett on 8/29/21.
//


#include "OutputHandler.h"


void OutputHandler::outputDirective(Directive d, const std::string &input) {
    switch(d) {
        case Directive::InitializeBlack:
            std::cout << "R B" << std::endl;
            break;
        case Directive::InitializeWhite:
            std::cout << "R W" << std::endl;
            break;
        case Directive::MoveSelf:
        case Directive::PassBlack:
        case Directive::PassWhite:
        case Directive::Comment:
            std::cout << input << std::endl;
            break;
    }
}

std::string OutputHandler::getMoveOutput(OthelloColor c, int pos) {

    // Pass
    if(pos < 0 || pos > 63) {
        return c == Black ? "B" : "W";
    }

    std::tuple<int, char> rowCol = Utils::posToRowCol(pos);

    int row = std::get<0>(rowCol);
    char col = std::get<1>(rowCol);

    char colorChar = c == Black ? 'B' : 'W';

    // Definitely overkill allocating 255, but we don't want to run into any unforseen errors due to the
    // string size being too small. So, this is just a safety measure. It's freed at the end anyway.
    char *str = (char *) malloc(sizeof(char) * 255);
    sprintf(str, "%c %c %d", colorChar, col, row);

    Logger::logComment("DEBUG: Converted pos " + std::to_string(pos) + " to " + str);

    std::string ret = std::string(str);
    free(str);
    return ret;
}

int OutputHandler::toPos(std::string input) {
    // Ex input: 'W a 1' -> returns 63 | 'B h 8' -> returns 0
    int x = 7 - Utils::getColFromChar(input[2]);
    int y = 8 - atoi(&input[4]);

    int pos = (y * 8) + x;

    if(pos < 0 || pos > 63) {
        // Can happen if we try to convert a standalone letter. e.g. W or B
        Logger::logComment("DEBUG: Received input '" + input + "'. Returning pos -1!");
        return -1;
    }

    Logger::logComment("DEBUG: Converted " + input + " to pos: " + std::to_string(pos));

    return pos;
}
