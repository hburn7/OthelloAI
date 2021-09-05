#include <bitset>
#include <string>

#include "Logger.h"

#include "Agent/Agent.h"

#include "IO/Input/InputHandler.h"
#include "IO/Output/OutputHandler.h"

#include "OthelloGame/BitBoard.h"
#include "OthelloGame/GameState.h"
#include "OthelloGame/OthelloGameBoard.h"

int main(int argc, char* argv[]) {
    // Init both sides.
    BitBoard black = BitBoard(OthelloColor::Black);
    BitBoard white = BitBoard(OthelloColor::White);
    OthelloGameBoard gameBoard = OthelloGameBoard(black, white);

    Logger::logComment("Gameboard initialized.");
    gameBoard.drawBoard();

    std::bitset<64> bBlack(black.getBits());
    std::bitset<64> bWhite(white.getBits());
    std::bitset<64> bCombined(black.getBits() | white.getBits());

    Logger::logComment("Black Binary: " + bBlack.to_string());
    Logger::logComment("White Binary: " + bWhite.to_string());
    Logger::logComment("Combined Binary: " + bCombined.to_string());

    Logger::logComment("Initializing player selection");
    Logger::logComment("Who would you like to play as?");

    auto input = InputHandler::readInput();
    auto directive = InputHandler::identifyDirective(input);

    Agent agent = Agent(OthelloColor::Black, black, white);
    if(directive == Directive::InitializeBlack) {
        Logger::logComment("Got it! I will play as black, you will play as white.");
        // Computer is already predefined as black, so we don't need to change anything.
    } else if(directive == Directive::InitializeWhite) {
        Logger::logComment("Got it! I will play as white, you will play as black.");
        agent = Agent(OthelloColor::White, white, black);
    } else {
        Logger::logComment("Something went wrong during initialization!");
        return EXIT_FAILURE;
    }

    OutputHandler::outputDirectiveConfirmation(directive, input);

    // Primary game loop
    bool playAsBlack = agent.getColor() == OthelloColor::Black;
    bool blackTurn = true;
    while(true) {
        std::string prompt = blackTurn ? "Black turn" : "White turn";

        Logger::logComment(prompt);

        std::string input = "";
        // Bot makes a move.
        if(playAsBlack && blackTurn || !playAsBlack && !blackTurn) {
            char colorChar = playAsBlack ? 'B' : 'W';
            auto moveSet = agent.getMove();
            int moveX = moveSet[0];
            int moveY = moveSet[1];

            input = std::to_string(colorChar) + " " + std::to_string(moveX) + " " + std::to_string(moveY);
        }
        else
        {
            // Player makes a move
            input = InputHandler::readInput();
        }
        auto directive = InputHandler::identifyDirective(input);

        // Output
        OutputHandler::outputDirectiveConfirmation(directive, input);
        blackTurn = -blackTurn;
    }

    return EXIT_SUCCESS;
}

OthelloColor oppositeColor(OthelloColor c) {
    switch(c) {
        case Black:
            return White;
        case White:
            return Black;
    }
}