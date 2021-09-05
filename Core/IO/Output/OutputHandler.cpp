//
// Created by Harry Burnett on 8/29/21.
//

#include <iostream>
#include "OutputHandler.h"

void OutputHandler::outputDirectiveConfirmation(Directive directive, const std::string &input) {
    switch(directive) {
        case Directive::InitializeBlack:
            std::cout << "R B" << std::endl;
            break;
        case Directive::InitializeWhite:
            std::cout << "R W" << std::endl;
            break;
        case Directive::MoveSelf:
        case Directive::MoveOpponent:
        case Directive::PassBlack:
        case Directive::PassWhite:
        case Directive::Comment:
        case Directive::EndGame:
            std::cout << input << std::endl;
            break;
        case Directive::Invalid:
            // Any input not detected by referee will cancel the game immediately.
            // Whatever is output here is arbitrary.
            std::cout << "<INVALID>" << std::endl;
            break;
    }
}

void OutputHandler::outputMove(OthelloColor color, std::array<int, 2> moveSet) {

}
