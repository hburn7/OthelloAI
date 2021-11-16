//
// Created by hburn7 on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_BITBOARD_H
#define OTHELLOPROJECT_CPP_BITBOARD_H

#include <algorithm>
#include <array>

#include "Move.h"
#include "../Logger.h"

/**
 * Binary representation of a gameboard, one for each color.
 */
class BitBoard {
public:
    /**
     * Initializes a bitboard for a given color.
     * @param color The color of the bitboard, either black or white. Color is to be pulled from Color.h
     */
    BitBoard(int color);
    /**
     * Copy constructor
     * @param oldBoard The old board that we are making a copy of.
     */
    BitBoard(BitBoard const &oldBoard);
    /**
     * Applies the move to the bitboard. All this does is set the bit to "on" that corresponds to
     * the move's pos value.
     * @param move The move on the board to apply.
     */
    void applyIsolatedMove(Move move);
    /**
     * Gets the state of a cell in the bitboard.
     * @param pos The position on the board, ranging 0-63 inclusive, that is being looked up.
     * @return True if the cell is populated with a piece (1 bit), False if empty (0 bit).
     */
    bool getCellState(int pos);
    /**
     * Gets the amount of on-bits in the board (the number of pieces allocated by the player).
     * @return The amount of non-empty pieces allocated on the bitboard.
     */
    int getCellCount();
    /**
     * @return The uint64_t representation of the board (decimal).
     */
    uint64_t getBits();
    /**
     * Sets this board's bits to the desired value.
     * @param bits Value to set this board's bits to.
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
