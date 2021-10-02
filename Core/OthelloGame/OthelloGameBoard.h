//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
#define OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H

#include <bitset>

#include "BitBoard.h"
#include "../Utils.h"

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
     * Captures opponent pieces in a line, flipping all necessary opponent pieces along the way.
     * @param color The color of the player making the action
     * @param newPos The most recent position selected on the board to move to.
     * @param previousState The previous state of the board, before newPos is applied.
     * @param curState The new state of the board, after newPos is applied.
     */
    void lineCap(OthelloColor color, int newPos);
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
private:
    BitBoard m_black;
    BitBoard m_white;

    /**
     * Returns a bitboard for a given color.
     * @param color The color of the board to retrieve.
     */
    BitBoard getBoard(OthelloColor color);
};


#endif //OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
