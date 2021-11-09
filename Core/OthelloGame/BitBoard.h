//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_BITBOARD_H
#define OTHELLOPROJECT_CPP_BITBOARD_H

#include <algorithm>
#include <array>

#include "Move.h"
#include "../Logger.h"

/**
 * Binary representation of a 2D array gameboard, one for each color. This class provides
 * wrapper methods ONLY to be accessed by the OthelloGameBoard class.
 */
class BitBoard {
public:
    /**
     * Initializes a bitboard for a given color.
     * @param color The color of the bitboard, either black or white.
     */
    BitBoard(int color);
    /**
     * Initializes a bitboard with a given color and bit configuration.
     * @param color The color of the bitboard, either black or white.
     * @param bits Binary representation of the gameboard for this color.
     */
    BitBoard(int color, uint64_t bits);
    /**
     * Copies a BitBoard into a new instance of a BitBoard.
     * @param oldBoard The old board that we are making a copy of.
     */
    BitBoard(BitBoard const &oldBoard);
    /**
     * Sets a single cell into an existing bitboard that may or may not be prepopulated.
     * @param move The move on the board to apply.
     * @return the updated board
     */
    void applyIsolatedMove(Move move);
    /**
     * Gets the state of a cell in the bitboard.
     * @param pos The position on the board, ranging 0-63 inclusive, that is being looked up.
     * @return True if the cell is populated with a piece, False if empty.
     */
    bool getCellState(int pos);
    /**
     * Gets the "score" of the board (the number of pieces allocated by the player).
     * @return The amount of non-empty pieces allocated on the bitboard.
     */
    int getCellCount();
    /**
     * Returns the bits for the board in the form of uint64_t.
     * @return A uint_64t containing the bits for this board.
     */
    uint64_t getBits();
    /**
     * Sets this boards bits to the desired value.
     * @param bits Value to update this board's bits to.
     */
    void setBits(uint64_t bits);
    /**
     * @return The color of the player who owns this board.
     */
    int getColor();

private:
    int color; // Either black or white
    uint64_t bits;
};


#endif //OTHELLOPROJECT_CPP_BITBOARD_H
