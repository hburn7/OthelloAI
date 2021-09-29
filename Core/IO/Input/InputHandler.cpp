//
// Created by Harry Burnett on 8/29/21.
//

#include <iostream>
#include "InputHandler.h"
#include "../../Logger.h"

std::string InputHandler::readInput() {
//    std::cout << ">> ";

    std::string input;
    std::getline(std::cin, input);
    return input;
}

Directive InputHandler::identifyDirective(std::string input, OthelloColor agentColor) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

    // All input is case sensitive.
    switch(input.length()) {
        default:
            if(input[0] == 'C') {
                return Directive::Comment;
            }

            if(Utils::parseInt(input) >= 0) {
                return Directive::EndGame;
            }

            return Directive::Invalid;
        case 1:
            if(input[0] == 'B') {
                return Directive::PassBlack;
            } else if(input[0] == 'W') {
                return Directive::PassWhite;
            }

            // Game conceded
            if(Utils::parseInt(input) >= 0) {
                return Directive::EndGame;
            }
            break;
        case 2:
            if(input == "IB") {
                return Directive::InitializeBlack;
            } else if(input == "IW") {
                return Directive::InitializeWhite;
            } else {
                // Game conceded
                if(Utils::parseInt(input) <= 64) { // Total num of spaces
                    return Directive::EndGame;
                }

                return Directive::Invalid;
            }
        case 3:
            // Check to see if str[1] is a letter a-h
            if(input.find_first_of("abcdefgh", 1) == std::string::npos) {
                return Directive::Invalid;
            }

            // Check to see if str[2] is a number 1-8
            if(input.find_first_of("12345678", 2) == std::string::npos) {
                return Directive::Invalid;
            }

            if(input[0] == 'B') {
                return agentColor == Black ? MoveSelf : MoveOpponent;
            } else if(input[0] == 'W') {
                return agentColor == White ? MoveSelf : MoveOpponent;
            }
            return Directive::Invalid;
    }

    return Directive::Invalid;
}