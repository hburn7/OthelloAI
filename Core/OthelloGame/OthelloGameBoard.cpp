//
// Created by Harry Burnett on 8/29/21.
//

#include <bitset>
#include <string.h>

#include "OthelloGameBoard.h"
#include "../Logger.h"

#define DIRECTION_COUNT 8
#define UNIVERSE 0xffffffffffffffffULL

const std::array<int, 8> DIR_INCREMENTS = {8, 9, 1, -7, -8, -9, -1, 7};
const std::array<uint64_t, 8> DIR_MASKS = {
    0xFFFFFFFFFFFFFF00L, //North
    0xFEFEFEFEFEFEFE00L, //NorthWest
    0xFEFEFEFEFEFEFEFEL, //West
    0x00FEFEFEFEFEFEFEL, //SouthWest
    0x00FFFFFFFFFFFFFFL, //South
    0x007F7F7F7F7F7F7FL, //SouthEast
    0x7F7F7F7F7F7F7F7FL, //East
    0x7F7F7F7F7F7F7F00L  //NorthEast
};

// todo: can be removed (all the array garbage)
OthelloGameBoard::OthelloGameBoard(BitBoard black, BitBoard white) : m_black(black), m_white(white) {
    this->m_board = std::array<std::array<int, 8>, 8> {};

    // Initializes an empty gameboard (8 x 8) to zeroes.
    this->m_board.fill(std::array<int, 8> { 0, 0, 0, 0, 0, 0, 0, 0 });

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int blackPos = getBlack().arrayRepresentation()[i][j];
            int whitePos = getWhite().arrayRepresentation()[i][j];

            if(blackPos != 0) {
                this->m_board[i][j] = blackPos;
            } else if(whitePos != 0) {
                this->m_board[i][j] = whitePos;
            }
        }
    }
}

std::array<std::array<int, 8>, 8> OthelloGameBoard::binaryToArray(uint64_t bitboardBits) {
    std::array<std::array<int, 8>, 8> board {};

    board.fill(std::array<int, 8> { 0, 0, 0, 0, 0, 0, 0, 0 });

    for(int i = 0; i < 64; i++) {
        if(((bitboardBits >> i) & 1) == 1) {
            board[i / 8][i % 8] = 1;
        }
    }

    return board;
}

void OthelloGameBoard::drawBoard() {
    drawBoard(this->getBlack().getBits(), this->getWhite().getBits());
}

void OthelloGameBoard::drawBoard(uint64_t black, uint64_t white) {
    Logger::logComment("    A B C D E F G H");
    Logger::logComment("    * * * * * * * *");

    auto blackBitset = std::bitset<64>(black);
    auto whiteBitset = std::bitset<64>(white);

    for(int i = 63; i >= 0; i--) {
        if(i % 8 == 7) {
            // We need to use manual prints in this specific case.
            std::cout << "C " << (-(i / 8) + 8) << " * ";
        }

        uint64_t mask = 1LL << i;
        bool blackBitSet = (black & mask) != 0;
        bool whiteBitSet = (white & mask) != 0;

        if(blackBitSet) {
            Logger::logComment("B ", false);
        } else if(whiteBitSet) {
            Logger::logComment("W ", false);
        } else {
            Logger::logComment("- ", false);
        }

        if(i % 8 == 0) {
            std::cout << std::endl;
        }
    }
}

BitBoard OthelloGameBoard::getBlack() {
    return this->m_black;
}

BitBoard OthelloGameBoard::getWhite() {
    return this->m_white;
}

std::array<std::array<int, 8>, 8> OthelloGameBoard::getBoard() {
    return this->m_board;
}

void OthelloGameBoard::applyMove(OthelloColor color, int pos) {
    BitBoard board = getBoard(color);
    board = board.setCellState(pos);

    // Update board internally.
    if(color == Black) {
        this->m_black = board;
    } else {
        this->m_white = board;
    }

    lineCap(color, pos);
}

BitBoard OthelloGameBoard::getBoard(OthelloColor color) {
    switch(color) {
        case Black:
            return this->m_black;
        case White:
            return this->m_white;
    }
}

bool OthelloGameBoard::isGameComplete() {
    return (this->getBlack().getBits() | this->getWhite().getBits()) == UNIVERSE;
}

int OthelloGameBoard::countPieces(OthelloColor color) {
    BitBoard toCount = color == Black ? this->getBlack() : this->getWhite();
    auto bitset = std::bitset<64>(toCount.getBits());
    return bitset.count();
}

// Credit to Dr. Mec for this algorithm.
uint64_t OthelloGameBoard::generateMoves(long playerDisks, long oppDisks) {
    // Returns move mask
    uint64_t emptyMask = ~playerDisks & ~oppDisks;
    uint64_t holdMask, dirMoveMask, moveMask = 0;

    for(int i = 0; i < DIRECTION_COUNT; i++) {
        // Finds opponent disks that are adjacent to player disks
        // in the current direction.
        holdMask = playerDisks;

        if(DIR_INCREMENTS[i] > 0) {
            holdMask = (holdMask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
        } else {
            holdMask = (holdMask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
        }
        holdMask = holdMask & oppDisks;

        // Find strings of 1 to 6 opponent disks that are adjacent
        // to the player in the current direction. Previous statement
        // has found the first opponent disk.

        for(int j = 0; ((j < 6) & (holdMask != 0LL)); j++) {
            if(DIR_INCREMENTS[i] > 0) {
                holdMask = (holdMask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
            } else {
                holdMask = (holdMask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
            }

            dirMoveMask = holdMask & emptyMask;
            moveMask |= dirMoveMask;
            holdMask &= (~dirMoveMask & oppDisks);
        }
    }

    return moveMask;
}

void OthelloGameBoard::lineCap(OthelloColor color, int newPos) {
    // Move is already applied when running this function.
    BitBoard self = color == Black ? this->getBlack() : this->getWhite();
    BitBoard opp = color == Black ? this->getWhite() : this->getBlack();

    uint64_t selfBits = self.getBits();
    uint64_t oppBits = opp.getBits();

    uint64_t move = 1LL << newPos;
    uint64_t f_fin = 0LL;
    uint64_t possibility;

    for(int i = 0; i < DIRECTION_COUNT; i++) {
        uint64_t to_change = 0LL;
        uint64_t search;

        if(DIR_INCREMENTS[i] > 0) {
            search = (move << DIR_INCREMENTS[i]) & DIR_MASKS[i];
        } else {
            search = (move >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
        }

        possibility = oppBits & search;

        // Keep moving forward, identifying bits to flip
        // If we are in this loop, there's an opponent piece here.

        while(possibility != 0LL) {
            to_change |= possibility;
            if(DIR_INCREMENTS[i] > 0) {
                search = (search << DIR_INCREMENTS[i]) & DIR_MASKS[i];
            } else {
                search = (search >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
            }

            if((selfBits & search) != 0) {
                f_fin |= to_change;
                break;
            }

            possibility = oppBits & search;
        }
    }

    selfBits |= f_fin;
    oppBits = (~f_fin) & oppBits;

    self.setBits(selfBits);
    opp.setBits(oppBits);

    this->m_black = color == Black ? self : opp;
    this->m_white = color == Black ? opp : self;
}
