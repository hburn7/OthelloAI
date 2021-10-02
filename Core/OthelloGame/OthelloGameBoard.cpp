//
// Created by Harry Burnett on 8/29/21.
//

#include "OthelloGameBoard.h"

#define DIRECTION_COUNT 8
#define UNIVERSE 0xffffffffffffffffULL

const std::array<int, 8> DIR_INCREMENTS = { 8, 9, 1, -7, -8, -9, -1, 7 };
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

// Assigns weights to every position on the board.
const std::array<int, 64> WEIGHT_MAP = {
        4, -3, 2, 2, 2, 2, -3, 4,
        -3, -4, -1, -1, -1, -1, -4, -3,
        2, -1, 1, 0, 0, 1, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 1, 0, 0, 1, -1, 2,
        -3, -4, -1, -1, -1, -1, -4, -3,
        4, -3, 2, 2, 2, 2, -3, 4
};

OthelloGameBoard::OthelloGameBoard(BitBoard black, BitBoard white) : m_black(black), m_white(white) {}

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
    return this->isGameComplete(this->getBlack().getBits(), this->getWhite().getBits());
}

bool OthelloGameBoard::isGameComplete(uint64_t playerDisks, uint64_t opponentDisks) {
    auto playerLegal = this->generateMoves(playerDisks, opponentDisks);
    auto oppLegal = this->generateMoves(opponentDisks, playerDisks);

    return ((playerLegal == 0 && oppLegal == 0) || (playerDisks | opponentDisks) == UNIVERSE);
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

    // 0 if no legal moves can be made.
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

int OthelloGameBoard::evaluate(uint64_t playerDisks, uint64_t opponentDisks) {
    int score = 0;
    int oppScore = 0;

    int mask = 1LL;
    for(int i = 0; i < 64; i++) {
        if(((mask << i) & playerDisks) != 0) {
            score += WEIGHT_MAP[i];
        }

        if(((mask << i) & opponentDisks) != 0) {
            oppScore += WEIGHT_MAP[i];
        }
    }

    if(score + oppScore != 0) {
        return 100 * ((score - oppScore) / (score + oppScore));
    }

    return 0;
}

int OthelloGameBoard::minimax(uint64_t playerDisks, uint64_t opponentDisks, int depth, int alpha, int beta, bool max) {
    if(depth == 0 || this->isGameComplete(playerDisks, opponentDisks)) {
        return this->evaluate(playerDisks, opponentDisks);
    }

    uint64_t possibleMoves = max
            ? this->generateMoves(playerDisks, opponentDisks)
            : this->generateMoves(opponentDisks, playerDisks);

    std::vector<int> children = {};
    for(int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if((mask & possibleMoves) != 0) {
            children.push_back(i);
        }
    }

    const uint64_t mask = 1LL;
    if(max) {
        // Maximizing player
        int maxEval = INT32_MIN;
        for(int child : children) {
            playerDisks |= (mask << child);
            int eval = minimax(playerDisks, opponentDisks, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if(beta <= alpha) {
                break;
            }
        }

        return maxEval;
    } else {
        // Minimizing player
        int minEval = INT32_MAX;
        for(int child : children) {
            opponentDisks |= (mask << child);
            int eval = minimax(playerDisks, opponentDisks, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if(beta <= alpha) {
                break;
            }
        }

        return minEval;
    }
}

int OthelloGameBoard::selectMove(uint64_t playerDisks, uint64_t opponentDisks) {
    uint64_t possibleMoves = this->generateMoves(playerDisks, opponentDisks);
    std::vector<int> children = {}; // possibleMoves in vector form
    std::vector<int> evaluations = {};

    for(int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if((mask & possibleMoves) != 0) {
            children.push_back(i);
        }
    }

    for(int _ : children) {
        evaluations.push_back(minimax(playerDisks, opponentDisks, 16, INT32_MIN, INT32_MAX, true));
    }

    // Iterate through child/evaluation score pairs and select best.
    int bestEval = INT32_MIN;
    int bestPos = INT32_MIN;

    for(int i = 0; i < evaluations.size(); i++) {
        int curEval = evaluations.at(i);
        int matchingChild = children.at(i);

        if(curEval > bestEval) {
            bestEval = curEval;
            bestPos = matchingChild;
        }
    }

    return bestPos;
}


