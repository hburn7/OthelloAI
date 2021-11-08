#include <bitset>
#include <string.h>

#include "Logger.h"

#include "IO/Input/InputHandler.h"
#include "IO/Output/OutputHandler.h"

#include "OthelloGame/BitBoard.h"
#include "OthelloGame/OthelloGameBoard.h"

// Time allotted for each player. Total game time is 2x this value.
#define DEF_MAX_TIME 60

int main(int argc, char* argv[]) {
    bool interactive = argc > 1 && strcmp(argv[1], "--interactive") == 0;
    int gameTime = argc > 2 ? std::stoi(argv[2]) : DEF_MAX_TIME;

    // Init config
    Config cfg = Config(interactive, gameTime);
    int agentColor;
    int opponentColor;

    std::string input;
    Directive directive;

    while(true) {
        Logger::logComment("Please initialize the agent color [I B] or [I W].");

        input = InputHandler::readInput();
        directive = InputHandler::identifyDirective(input, BLACK);

        if(directive == Directive::InitializeBlack) {
            Logger::logComment("Got it! I will play as black, you will play as white.");

            agentColor = BLACK;
            opponentColor = WHITE;

            break;
        } else if(directive == Directive::InitializeWhite) {
            Logger::logComment("Got it! I will play as white, you will play as black.");

            agentColor = WHITE;
            opponentColor = BLACK;

            break;
        } else {
            Logger::logComment("Something went wrong during initialization!");
            Logger::logComment("Expected: 'I B' or 'I W' -- received: " + input);
        }
    }

    BitBoard agentBoard = BitBoard(agentColor);
    BitBoard opponentBoard = BitBoard(opponentColor);
    OthelloGameBoard gameBoard = OthelloGameBoard(cfg, agentColor, agentBoard, opponentBoard);

    Logger::logComment("Gameboard initialized.");
    gameBoard.drawBoard();

    OutputHandler::outputDirective(directive, input);

    // Primary game loop
    bool playAsBlack = agentColor == BLACK;
    bool blackTurn = true;

    while(!gameBoard.isGameComplete()) {
        agentBoard = gameBoard.getPlayer();
        opponentBoard = gameBoard.getOpponent();

        std::string prompt = blackTurn ? "Black turn" : "White turn";
        Logger::logComment(prompt);

        Directive newDirective;

        bool agentTurn = playAsBlack && blackTurn || !playAsBlack && !blackTurn;

        // Agent makes a move.
        if(agentTurn) {
            Move move = gameBoard.selectMove(agentColor, false);

            // Apply move to board if not passing
            if(!move.isPass()) {
                gameBoard.applyMove(agentBoard, move);
            }

            input = OutputHandler::getMoveOutput(agentColor, move, true);
            newDirective = InputHandler::identifyDirective(input, agentColor);
        }
        else
        {
            Move move;
            if(cfg.isInteractive()) {
                uint64_t possibleMoves = gameBoard.generateMoveMask(opponentBoard.getBits(), agentBoard.getBits());

                // Player prompted to make a move
                input = InputHandler::readInput();
                move = OutputHandler::toMove(input);

                // Compare player move to any legal move. We do not allow illegal moves.                  -- Checks for legal pass --
                bool valid = (possibleMoves > 0 && ((1LL << move.getPos()) & possibleMoves) != 0) || (move.isPass() && possibleMoves == 0);

                while(!valid) {
                    Logger::logComment("Invalid move, please try again.");

                    input = InputHandler::readInput();
                    move = OutputHandler::toMove(input);

                    valid = ((1LL << move.getPos()) & possibleMoves) != 0;
                }

            } else {
                // "Player" (agent) makes a move if not m_interactive
                move = gameBoard.selectMove(opponentColor, true);
            }

            // Apply move to board if not passing.
            if(!move.isPass()) {
                gameBoard.applyMove(opponentBoard, move);
            }

            input = OutputHandler::getMoveOutput(opponentColor, move, true);
            newDirective = InputHandler::identifyDirective(input, opponentColor);
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
        Logger::logComment("Score (Black): " + std::to_string(gameBoard.getForColor(BLACK).getCellCount()));
        Logger::logComment("Score (White): " + std::to_string(gameBoard.getForColor(WHITE).getCellCount()));
    }

    // Output end-game result
    int f_black = gameBoard.countPieces(BLACK);
    std::cout << f_black << std::endl;

    return EXIT_SUCCESS;
}