#include <bitset>
#include <string.h>

#include "Logger.h"

#include "Agent/Agent.h"

#include "IO/Input/InputHandler.h"
#include "IO/Output/OutputHandler.h"

#include "OthelloGame/BitBoard.h"
#include "OthelloGame/OthelloGameBoard.h"

int main(int argc, char* argv[]) {
    bool interactive = argc > 1 && strcmp(argv[1], "-interactive") == 0;

    // Init both sides.
    BitBoard agentBoard = BitBoard(Black);
    BitBoard playerBoard = BitBoard(White);

    OthelloGameBoard gameBoard = OthelloGameBoard(agentBoard, playerBoard);

    Logger::logComment("Gameboard initialized.");
    gameBoard.drawBoard();

    OthelloColor agentColor = Black;
    OthelloColor playerColor = White;

    std::string input;
    Directive directive;

    while(true) {
        Logger::logComment("Please initialize the agent color [I B] or [I W].");

        input = InputHandler::readInput();
        directive = InputHandler::identifyDirective(input, Black);

        if(directive == Directive::InitializeBlack) {
            Logger::logComment("Got it! I will play as black, you will play as white.");
            // Computer is already predefined as black, so we don't need to change anything.

            break;
        } else if(directive == Directive::InitializeWhite) {
            Logger::logComment("Got it! I will play as white, you will play as black.");

            agentColor = White;
            playerColor = Black;

            break;
        } else {
            Logger::logComment("Something went wrong during initialization!");
            Logger::logComment("Expected: 'I B' or 'I W' -- received: " + input);
        }
    }

    OutputHandler::outputDirective(directive, input);

    // Primary game loop
    bool playAsBlack = agentColor == Black;
    bool blackTurn = true;

    while(!gameBoard.isGameComplete()) {
        agentBoard = agentColor == Black ? gameBoard.getBlack() : gameBoard.getWhite();
        playerBoard = playerColor == Black ? gameBoard.getBlack() : gameBoard.getWhite();

        std::string prompt = blackTurn ? "Black turn" : "White turn";
        Logger::logComment(prompt);

        Directive newDirective;

        bool agentTurn = playAsBlack && blackTurn || !playAsBlack && !blackTurn;

        // Agent makes a move.
        if(agentTurn) {
            int move = gameBoard.selectMove(agentBoard.getBits(), playerBoard.getBits());

            // Apply move to board if not passing
            if(move >= 0) {
                gameBoard.applyMove(agentColor, move);
            }

            input = OutputHandler::getMoveOutput(agentColor, move);
            newDirective = InputHandler::identifyDirective(input, agentColor);
        }
        else
        {
            int move;
            uint64_t possibleMoves = gameBoard.generateMoves(playerBoard.getBits(), agentBoard.getBits());

            if(interactive) {
                // Player prompted to make a move
                input = InputHandler::readInput();
                move = OutputHandler::toPos(input);

                // Compare player move to any legal move. We do not allow illegal moves.        -- Checks for legal pass --
                bool valid = (possibleMoves > 0 && ((1LL << move) & possibleMoves) != 0) || (move == -1 && possibleMoves == 0);

                while(!valid) {
                    Logger::logComment("Invalid move, please try again.");

                    input = InputHandler::readInput();
                    move = OutputHandler::toPos(input);

                    valid = ((1LL << move) & possibleMoves) != 0;
                }

            } else {
                // "Player" (agent) makes a move if not interactive
                move = gameBoard.selectMove(playerBoard.getBits(), agentBoard.getBits());
            }

            // Apply move to board if not passing.
            if(move >= 0) {
                gameBoard.applyMove(playerColor, move);
            }

            input = OutputHandler::getMoveOutput(playerColor, move);
            newDirective = InputHandler::identifyDirective(input, playerColor);
        }

        // Output
        if(agentTurn) {
            OutputHandler::outputDirective(newDirective, input);
        }

        if(newDirective == Directive::Invalid) {
            Logger::logComment("Invalid move! Try again.");
            continue;
        }

        gameBoard.drawBoard();
        blackTurn = !blackTurn;

        // Score Tracking
        Logger::logComment("Score (Black): " + std::to_string(gameBoard.getBlack().getCellCount()));
        Logger::logComment("Score (White): " + std::to_string(gameBoard.getWhite().getCellCount()));
    }

    // Output end-game result
    int f_black = gameBoard.countPieces(Black);
    std::cout << f_black << std::endl;

    return EXIT_SUCCESS;
}