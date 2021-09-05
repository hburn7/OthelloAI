//
// Created by Harry Burnett on 8/29/21.
//

#include <iostream>
#include "InputHandler.h"
#include "../../Logger.h"

std::string InputHandler::readInput() {
    std::cout << ">> ";

    std::string input;
    //std::cin.ignore();
    std::getline(std::cin, input);
    return input;
}

Directive InputHandler::identifyDirective(std::string input) {
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());

    // All input is case sensitive.
    switch(input.length()) {
        default:
            if(input[0] == 'C') {
                return Directive::Comment;
            }

            if(parseInt(input) != -1) {
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
            if(parseInt(input) != -1) {
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
                if(parseInt(input) != -1) {
                    return Directive::EndGame;
                }

                return Directive::Invalid;
            }
        case 3:
            // Check to see if str[1] is a letter a-h
            if(input.find_first_not_of("abcdefgh", 1, 1) == std::string::npos) {
                return Directive::Invalid;
            }

            // Check to see if str[2] is a number 1-8
            if(input.find_first_not_of("12345678", 2, 1) == std::string::npos) {
                return Directive::Invalid;
            }

            switch(input[0]) {
                case 'B':
                    return Directive::MoveSelf;
                case 'W':
                    return Directive::MoveOpponent;
            }

            // Game Conceded
            if(parseInt(input) != -1) {
                return Directive::EndGame;
            }

            return Directive::Invalid;
    }
}

int InputHandler::parseInt(const std::string &input) {
    try {
        return std::stoi(input);
    } catch(std::exception &e) {
        return -1;
    }
}

int InputHandler::charToInt(char column) {
    std::string columns = "abcdefgh";
    for(int i = 0; i < columns.size(); i++) {
        char cur = columns[i];
        if(cur == column) {
            return i;
        }
    }

    return -1;
}
