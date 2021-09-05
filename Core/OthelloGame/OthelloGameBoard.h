//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
#define OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H

#include "BitBoard.h"

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
     * Converts an 8x8 othello gameboard into bitboards for an arbitrary color. 0 = empty, non-0 = occupied.
     * @param bitboardBits Bitboard representation of empty/occupied pieces for a given color. Non-0 = occupied.
     * @return long long representing occupied/empty pieces for the given input.
     */
    static long long arrayToBinary(std::array<std::array<int, 8>, 8> pieces);
    static std::array<std::array<int, 8>, 8> binaryToArray(long long bitboardBits);
    /**
     * Creates an 8 x 8 OthelloGameBoard (data represented as a 2-dimensional std::array of integers)
     * from two longs, b and w.
     * @param b Binary representation of all black pieces on the m_board.
     * @param w Binary representation of all white pieces on the m_board.
     * @return
     */
    static OthelloGameBoard createBoard(long long b, long long w);
    /**
     * Prints out the gameboard as a referee-compatible comment.
     */
    void drawBoard();
    BitBoard getBlack();
    BitBoard getWhite();
    /**
     * @return A 2-dimensional int[] containing non-zero values for occupied spaces.
     */
    std::array<std::array<int, 8>, 8> getBoard();
private:
    BitBoard m_black;
    BitBoard m_white;
    bool is_finished;
    std::array<std::array<int, 8>, 8> m_board{};
    /**
     * Takes a given string, represented in 64-bit binary form, and converts it into a long.
     * @param binary 64-bit binary number, as a string.
     * @return Binary input converted to a base-10 long.
     */
    static long long stringToBinary(std::string binary);
};


#endif //OTHELLOPROJECT_CPP_OTHELLOGAMEBOARD_H
