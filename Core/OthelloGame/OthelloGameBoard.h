//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
#define OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H

#include <bitset>
#include <chrono>
#include <math.h>
#include <queue>
#include <vector>
#include <stack>

#include "BitBoard.h"
#include "Color.h"
#include "Move.h"
#include "../Config.h"
#include "../Utils.h"
#include "../IO/Output/OutputHandler.h"

// TODO: Update comments - a lot of the docs here are outdated.

/**
 * Represents the entire gameboard for both black and white. When working
 * in the main program, this is what we want to be manipulating. The
 * BitBoard class serves as a wrapper with loads of internal methods NOT
 * to be accessed outside of this class.
 *
 * This class provides a layer of abstraction when it comes to modifying the gameboard.
 */
class OthelloGameBoard {
public:
    /**
     * Initializes the gameboard to the specified values.
     * @param cfg The internal configuration for the game
     * @param player Initial player starting m_board
     * @param opponent Initial opponent starting m_board
     */
    OthelloGameBoard(Config cfg, int playerColor, BitBoard player, BitBoard opponent);
    /**
     * Creates a copy of gameboard.
     * @param gameBoard The board to copy.
     */
    OthelloGameBoard(const OthelloGameBoard &gameBoard);
    /**
     * Generates a bitmask containing all possible moves for the given player.
     * @param playerDisks Player's pieces
     * @param oppDisks Opponent's pieces
     * @return A uint64_t containing all possible moves for the color.
     */
    uint64_t generateMoveMask(uint64_t playerDisks, uint64_t oppDisks);
    /**
     * Selects a move for the given player.
     * @param playerColor The color of the player the move is being selected for.
     * @param random Whether or not the move returned is a pseudo-random move.
     * @return An optimal move, using alphaBeta and alpha-beta pruning.
     */
    Move selectMove(int playerColor, bool random);
    /**
     * Applies a move to the game board for the given color. Row and column are indexed from zero.
     * @param board The board to apply the move to.
     * @param m The move to apply
     * The position to set the move at, ranging 0-63 inclusive.
     */
    void applyMove(BitBoard board, Move m);
    /**
     * Prints out the current game board to the console.
     */
    void drawBoard();
    /**
    * Draws the board with a set of data.
    */
    static void drawBoard(OthelloGameBoard gameBoard);
    BitBoard getPlayer();
    BitBoard getOpponent();
    BitBoard getForColor(int color);
    /**
     * Updates a bitboard inside the gameboard based on its color.
     * @param board The board to update internally, based on color.
     */
    void setForColor(BitBoard board);
    int getPlayerColor();
    int getOpponentColor();

    const Config &getCfg() const;
    /**
     * Returns true if the game is complete, false if the game is ongoing.
     * Uses the total count of occupied cells on the board to determine this.
     */
    bool isGameComplete();
    /**
     * Counts the number of occupied cells for a given color.
     * @param color The color to count
     * @return The number of pieces occupied
     */
    int countPieces(int color);
    /**
     * @return Total amount of set bits in the provided data.
     */
    int countBits(uint64_t bits);
private:
    Config m_cfg;
    int m_playerColor;
    BitBoard m_playerBoard;
    BitBoard m_opponentBoard;

    /**
     * Returns a bitboard for a given color.
     * @param color The color of the board to retrieve.
     */
    BitBoard getBoard(int color);
    /**
     * Captures opponent pieces in a line, flipping all necessary opponent pieces along the way.
     * @param color The color of the player making the action
     * @param newMove The most recent position selected on the board to move to.
     */
    void lineCap(BitBoard board, Move newMove);
    /**
     * Performs a minimax search with alpha-beta pruning.
     * @param gameBoard The current state of the game.
     * @param player The maximizing player. 1 is the AI (max), -1 is the opponent (min).
     * @param depth The current search depth.
     * @param maxDepth The maximum depth to search to.
     * @param stopTime The real system time at which to abort evaluation.
     * @param alpha The lowest value that max player will accept.
     * @param beta The highest value that min player will accept.
     * @return Pair with the score and depth of the evaluation.
     */
    std::pair<int, int> alphaBeta(OthelloGameBoard gameBoard, int player, int depth, int maxDepth,
                  uint64_t stopTime, int alpha, int beta, bool max);
    /**
     * Helper function to return a priority queue of board positions for a given board state.
     * @return A priority queue of moves available based on the state.
     * The priority queue's first value is the rank of the move, the second value
     * is the integer value of the move (ranged 0-63 inclusive).
     */
    std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>> getMovesAsPriorityQueue(uint64_t state);
    /**
     * Generates moves and returns them as a priority queue.
     * @param player The player to generate moves for.
     * @param opponent Current state of opponent board.
     * @return Priority queue of moves for player, ranked from most to least beneficial.
     * The priority queue's first value is the rank of the move, the second value
     * is the integer value of the move (ranged 0-63 inclusive).
     */
    std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>> generateMovesAsPriorityQueue(BitBoard player, BitBoard opponent);
    /**
      * Scores the given board state and returns the value.
      * A positive score means the board at the given configuration favors the player.
      * A negative score means the board favors the opponent.
      * @param playerDisks The player's disks.
      * @param opponentDisks The opponent's disks.
      * @return A score reflective of how much the board is in favor of our player.
      */
    int evaluate(OthelloGameBoard gameBoard);
    /**
    * Determines whether the game would be finished if the given disk states
    * are to be applied to the board.
    */
    bool isGameComplete(OthelloGameBoard gameBoard);
    /**
     * @return Current system time in milliseconds
     */
    uint64_t getCurrentSysTime();
};


#endif //OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
