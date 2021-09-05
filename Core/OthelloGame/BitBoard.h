//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_BITBOARD_H
#define OTHELLOPROJECT_CPP_BITBOARD_H

#include <array>
#include "Direction.h"
#include "OthelloColor.h"

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
    BitBoard(OthelloColor color, long long bits);
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
    BitBoard(BitBoard const &oldBoard, long long bits);
    /**
     * Converts the binary bitboard into a 2D array.
     * @return A 2D array representation of this bitboard.
     */
    std::array<std::array<int, 8>, 8> arrayRepresentation();

    /**
     * Sets a single cell into an existing bitboard that may or may not be prepopulated.
     * @param row The row to insert to, indexed from 0.
     * @param col The column to insert to, indexed from 0.
     * @return A bitboard containing the newly inserted state.
     */
    BitBoard setCellState(int row, int col);
    /**
     * Gets the state of a cell in the bitboard.
     * @param row The row to read from, indexed from 0.
     * @param col The column to read from, indexed from 0.
     * @return True if the cell is populated with a piece, False if empty.
     */
    bool getCellState(int row, int col);
    /**
     * Gets the "score" of the board (the number of pieces allocated by the player).
     * @return The amount of non-empty pieces allocated on the bitboard.
     */
    int getCellCount();
    long long getBits();
    OthelloColor getColor();

    // TODO: Make an array with all directions.
    // TODO: i.e. arr[0] = North arr[1] = NE arr[2] = E, etc...

    /**
     * Returns a std::array<int, 8> containing bitwise shift amounts for
     * every direction on the game board.
     * @return Array containing all directions as follows:
     * North = 0
     * North East = 1
     * East = 2
     * South East = 3
     * South = 4
     * South West = 5
     * West = 6
     * North West = 7
     */
    static std::array<int, 8> shifts();

private:
    OthelloColor color; // Either black or white
    long long bits;

    BitBoard initBlack();
    BitBoard initWhite();

    std::tuple<Direction, int> shiftBits;

    /**
     * Determines how many bit positions to shift for a given direction
     * @param direction
     * @return
     */
    int shift(Direction direction);
};


#endif //OTHELLOPROJECT_CPP_BITBOARD_H
