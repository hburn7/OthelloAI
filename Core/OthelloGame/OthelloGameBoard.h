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

#include "BitBoard.h"
#include "../Utils.h"
#include "../Agent/Agent.h"
#include "../IO/Output/OutputHandler.h"

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
     * Initializes the game m_board to default values, except for the provided BitBoards which are
     * assumed to have pre-populated values.
     * @param black Initial black starting m_board
     * @param white Initial white starting m_board
     */
    OthelloGameBoard(BitBoard black, BitBoard white);
    /**
     * Generates a uint64_t containing all possible moves for the given player.
     * @param playerDisks Player's pieces
     * @param oppDisks Opponent's pieces
     * @return A uint64_t containing all possible moves for the color.
     */
    uint64_t generateMoves(long playerDisks, long oppDisks);
    /**
     * Selects a move for the given player.
     * @param playerColor The color of the player.
     * @param playerDisks The disks belonging to the player the move is generated for.
     * @param opponentDisks The disks belonging to the opponent player.
     * @param maxDepth The maximum depth to search down the game tree.
     * @return An optimal move, using minimax and alpha-beta pruning.
     */
    int selectMove(OthelloColor playerColor, uint64_t playerDisks, uint64_t opponentDisks, int maxDepth);
    /**
     * Applies a move to the game board for the given color. Row and column are indexed from zero.
     * @param color The color of the player we are applying this move for.
     * The position to set the move at, ranging 0-63 inclusive.
     */
    void applyMove(OthelloColor color, int pos);
    /**
     * Prints out the current game board to the console.
     */
    void drawBoard();
    /**
    * Draws the board with a set of data.
    */
    static void drawBoard(uint64_t black, uint64_t white);
    BitBoard getBlack();
    BitBoard getWhite();
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
    int countPieces(OthelloColor color);
    /**
     * @return Total amount of set bits in the provided data.
     */
    int countBits(uint64_t bits);
private:
    BitBoard m_black;
    BitBoard m_white;

    int m_blackSecondsRemaining;
    int m_whiteSecondsRemaining;

    /**
     * Returns a bitboard for a given color.
     * @param color The color of the board to retrieve.
     */
    BitBoard getBoard(OthelloColor color);
    /**
     * Captures opponent pieces in a line, flipping all necessary opponent pieces along the way.
     * @param color The color of the player making the action
     * @param newPos The most recent position selected on the board to move to.
     * @param previousState The previous state of the board, before newPos is applied.
     * @param curState The new state of the board, after newPos is applied.
     */
    void lineCap(OthelloColor color, int newPos);
    /**
     * Performs a minimax search algorithm, producing a game tree, using alpha-beta pruning.
     * @param depth The current search depth.
     * @param maxDepth The maximum depth to search.
     * @param alpha Best possible score maximizing player can achieve.
     * @param beta Best possible score minimizing player can achieve.
     * @param maximizingPlayer Whether we are evaluating the maximizing player or minimizing player.
     * @param playerDisks The player's disks as they appear down the tree.
     * @param opponentDisks The opponent's disks as they appear down the tree.
     */
    int minimax(int pos, uint64_t playerDisks, uint64_t opponentDisks, int depth, int maxDepth, int alpha, int beta, bool maximizingPlayer);
    /**
     * Helper function to return a priority queue of board positions for a given board state.
     * @return A priority queue of integer pairs where the first integer in the pair is the weight of the move and
     * the second int is the position on the board to move to.
     */
    std::priority_queue<std::pair<int, int>> getMovesAsPriorityQueue(uint64_t state);
    /**
      * Scores the given board state and returns the value.
      * A positive score means the board at the given configuration favors the agent.
      * A negative score means the board favors the opponent.
      * @param playerDisks The player's disks.
      * @param opponentDisks The opponent's disks.
      * @return A score reflective of how much the board is in favor of our agent.
      */
    int evaluate(uint64_t playerDisks, uint64_t opponentDisks);
    /**
    * Determines whether the game would be finished if the given disk states
    * are to be applied to the board.
    */
    bool isGameComplete(uint64_t playerDisks, uint64_t opponentDisks);
};


#endif //OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
