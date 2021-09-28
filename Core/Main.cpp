#include <bitset>
#include <string>

#include "Logger.h"

#include "Agent/Agent.h"

#include "IO/Input/InputHandler.h"
#include "IO/Output/OutputHandler.h"

#include "OthelloGame/BitBoard.h"
#include "OthelloGame/OthelloGameBoard.h"

int main(int argc, char* argv[]) {
    bool interactive = argc > 0 && argv[1] == "-interactive";

    // Init both sides.
    BitBoard black = BitBoard(Black);
    BitBoard white = BitBoard(White);
    OthelloGameBoard gameBoard = OthelloGameBoard(black, white);

    Logger::logComment("Gameboard initialized.");
    gameBoard.drawBoard();

    auto input = InputHandler::readInput();
    auto directive = InputHandler::identifyDirective(input, Black);

    OthelloColor agentColor = Black;
    OthelloColor playerColor = White;

    BitBoard agentBoard = black;
    BitBoard playerBoard = white;

    Logger::logComment("Initializing playerAgent selection");
    Logger::logComment("Please initialize the agent color.");

    if(directive == Directive::InitializeBlack) {
        Logger::logComment("Got it! I will play as black, you will play as white.");
        // Computer is already predefined as black, so we don't need to change anything.
    } else if(directive == Directive::InitializeWhite) {
        Logger::logComment("Got it! I will play as white, you will play as black.");

        agentColor = White;
        playerColor = Black;

        agentBoard = white;
        playerBoard = black;
    } else {
        Logger::logComment("Something went wrong during initialization!");
        return EXIT_FAILURE;
    }

    OutputHandler::outputDirective(directive, input);

    // Primary game loop
    bool playAsBlack = agentColor == Black;
    bool blackTurn = true;

    Agent agent = Agent(agentColor);
    Agent playerAgent = Agent(playerColor);

    while(!gameBoard.isGameComplete()) {
        agentBoard = agentColor == Black ? gameBoard.getBlack() : gameBoard.getWhite();
        playerBoard = playerColor == Black ? gameBoard.getBlack() : gameBoard.getWhite();

        std::string prompt = blackTurn ? "Black turn" : "White turn";
        Logger::logComment(prompt);

        std::string input;
        Directive directive;

        // Agent makes a move.
        if(playAsBlack && blackTurn || !playAsBlack && !blackTurn) {
            uint64_t possibleMoves = gameBoard.generateMoves(agentBoard.getBits(), playerBoard.getBits());
            int selection = agent.selectMove(possibleMoves);

            gameBoard.applyMove(agentColor, selection);
            gameBoard.lineCap(agentColor, selection);

            std::string out = OutputHandler::getMoveOutput(agentColor, selection);

            input = out;
            directive = InputHandler::identifyDirective(input, agentColor);
        }
        else
        {
            int move;

            if(interactive) {
                // Player prompted to make a move
                input = InputHandler::readInput();
                move = OutputHandler::toPos(input);
            } else {
                // "Player" agent makes a move
                uint64_t possibleMoves = gameBoard.generateMoves(playerBoard.getBits(), agentBoard.getBits());
                move = playerAgent.selectMove(possibleMoves);
            }

            gameBoard.applyMove(playerColor, move);
            gameBoard.lineCap(playerColor, move);
            input = OutputHandler::getMoveOutput(playerColor, move);
            directive = InputHandler::identifyDirective(input, playerColor);
        }

        // Output
        OutputHandler::outputDirective(directive, input);

        if(directive == Directive::EndGame) {
            // Score would have been printed by outputDirective.
            if(gameBoard.isGameComplete()) {
                return EXIT_SUCCESS;
            }

            return EXIT_FAILURE;
        }

        if(directive == Directive::Invalid) {
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