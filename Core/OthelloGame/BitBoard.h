//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_BITBOARD_H
#define OTHELLOPROJECT_CPP_BITBOARD_H

#include <array>
#include "../Logger.h"
#include "OthelloColor.h"

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
    BitBoard(OthelloColor color);
    /**
     * Initializes a bitboard with a given color and bit configuration.
     * @param color The color of the bitboard, either black or white.
     * @param bits Binary representation of the gameboard for this color.
     */
    BitBoard(OthelloColor color, uint64_t bits);
    /**
     * Copies a BitBoard into a new instance of a BitBoard.
     * @param oldBoard The old board that we are making a copy of.
     */
    BitBoard(BitBoard const &oldBoard);
    /**
     * Updates an existing bitboard with a new set of data.
     * @param oldBoard The board to update.
     * @param bits The new set of data (bits) to update.
     */
    BitBoard(BitBoard const &oldBoard, uint64_t bits);
    /**
     * Converts the binary bitboard into a 2D array.
     * @return A 2D array representation of this bitboard.
     */
    std::array<std::array<int, 8>, 8> arrayRepresentation();

    /**
     * Sets a single cell into an existing bitboard that may or may not be prepopulated.
     * @param pos The position on the board, ranging 0-63 inclusive, to set on the board.
     * @return the updated board
     */
    BitBoard setCellState(int pos);
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
    uint64_t getBits();
    void setBits(uint64_t bits);
    OthelloColor getColor();

private:
    OthelloColor color; // Either black or white
    uint64_t bits;
};


#endif //OTHELLOPROJECT_CPP_BITBOARD_H
