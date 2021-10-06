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
        20, -3, 11, 8, 8, 11, -3, 20,
        -3, -7, -4, 1, 1, -4, -7, -3,
        11, -4, 2, 2, 2, 2, -4, 11,
        8, 1, 2, -3, -3, 2, 1, 8,
        8, 1, 2, -3, -3, 2, 1, 8,
        11, -4, 2, 2, 2, 2, -4, 11,
        -3, -7, -4, 1, 1, -4, -7, -3,
        20, -3, 11, 8, 8, 11, -3, 20
};

//const std::array<int, 64> WEIGHT_MAP = {
//        10, -3, 2, 2, 2, 2, -3, 10,
//        -3, -4, -1, -1, -1, -1, -4, -3,
//        2, -1, 1, 0, 0, 1, -1, 2,
//        2, -1, 0, 1, 1, 0, -1, 2,
//        2, -1, 0, 1, 1, 0, -1, 2,
//        2, -1, 1, 0, 0, 1, -1, 2,
//        -3, -4, -1, -1, -1, -1, -4, -3,
//        10, -3, 2, 2, 2, 2, -3, 10
//};

const uint64_t CORNER_MASK = 0x8100000000000081; // Corner spaces
const uint64_t CORNER_ADJACENT_MASK = 0x42C3000000C342; // Spaces directly adjacent to a corner

OthelloGameBoard::OthelloGameBoard(BitBoard black, BitBoard white) : m_black(black), m_white(white) {}

void OthelloGameBoard::drawBoard() {
    drawBoard(this->getBlack().getBits(), this->getWhite().getBits());
}

void OthelloGameBoard::drawBoard(uint64_t black, uint64_t white) {
    Logger::logComment("    A B C D E F G H");
    Logger::logComment("    * * * * * * * *");

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

int OthelloGameBoard::countBits(uint64_t bits) {
    std::bitset<64> bitset = std::bitset<64>(bits);
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
    double p = 0, c = 0, l = 0, m = 0, d = 0; // todo: rename variables

    int countPlayer = this->countBits(playerDisks);
    int countOpponent = this->countBits(opponentDisks);

    uint64_t playerPossibleMoves = this->generateMoves(playerDisks, opponentDisks);
    uint64_t opponentPossibleMoves = this->generateMoves(opponentDisks, playerDisks);

    // Individual weights
    for(int i = 0; i < 64; i++) {
        uint64_t shift = 1LL << i;
        if((shift & playerDisks) != 0) {
            d += WEIGHT_MAP[i];
        } else if((shift & opponentDisks) != 0) {
            d -= WEIGHT_MAP[i];
        }
    }

    // Coin parity
    int countMovesPlayer = this->countBits(playerPossibleMoves);
    int countMovesOpponent = this->countBits(opponentPossibleMoves);

    if(countPlayer > countOpponent) {
        p = (100.0 * countPlayer) / (countPlayer + countOpponent);
    } else if(countPlayer < countOpponent) {
        p = -(100.0 * countOpponent) / (countPlayer + countOpponent);
    } else {
        p = 0;
    }

    // Corners
    auto playerCornerBitset = std::bitset<64>(playerDisks & CORNER_MASK);
    auto opponentCornerBitset = std::bitset<64>(opponentDisks & CORNER_MASK);

    auto playerAdjacentCornerBitset = std::bitset<64>(playerDisks & CORNER_ADJACENT_MASK);
    auto opponentAdjacentCornerBitset = std::bitset<64>(opponentDisks & CORNER_ADJACENT_MASK);

    c = 25 * (playerCornerBitset.count() - opponentCornerBitset.count());

    // Corner closeness (these are bad spots to be in)
    l = -(12.5 * (playerAdjacentCornerBitset.count() - opponentAdjacentCornerBitset.count()));

    // Mobility
    if(countMovesPlayer > countMovesOpponent) {
        m = (100.0 * countMovesPlayer) / (countMovesPlayer + countMovesOpponent);
    } else if(countMovesPlayer < countMovesOpponent) {
        m = -(100.0 * countMovesPlayer) / (countMovesPlayer + countMovesOpponent);
    } else {
        m = 0;
    }

    // Calculate heuristic
    double score = (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (10 * d);
    return (int)score;
}

int OthelloGameBoard::minimax(uint64_t playerDisks, uint64_t opponentDisks, int depth, int maxDepth,
                              int alpha, int beta, bool maximizingPlayer) {

    uint64_t children = maximizingPlayer
            ? this->generateMoves(playerDisks, opponentDisks)
            : this->generateMoves(opponentDisks, playerDisks);

    std::vector<int> childVector = this->getMovesAsVector(children);

    if(depth == maxDepth || children == 0 || this->isGameComplete(playerDisks, opponentDisks)) {
        int evaluation = this->evaluate(playerDisks, opponentDisks);
        return evaluation;
    }

    if(maximizingPlayer) {
        int maxEval = INT32_MIN;

        for(int child : childVector) {
            playerDisks |= (1LL << child);
            int eval = minimax(playerDisks, opponentDisks, depth + 1, maxDepth, alpha, beta, !maximizingPlayer);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);

            if(beta <= alpha) {
                break;
            }
        }

        // todo: fix bug(s) where maxEval is returned while it is still equal to INT32_MIN
        return maxEval;
    } else {
        int minEval = INT32_MAX;

        for(int child : childVector) {
            opponentDisks |= (1LL << child);
            int eval = minimax(opponentDisks, playerDisks, depth + 1, maxDepth, alpha, beta, !maximizingPlayer);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);

            if(beta <= alpha) {
                break;
            }
        }

        return minEval;
    }
}

int OthelloGameBoard::selectMove(OthelloColor playerColor, uint64_t playerDisks, uint64_t opponentDisks, int maxDepth) {
    uint64_t playerPossible = this->generateMoves(playerDisks, opponentDisks);

    if(playerPossible == 0) {
        Logger::logComment("Passing, no moves to make for player.");
        return -1; // Pass, cannot make any moves
    }

    std::vector<int> pChildren = this->getMovesAsVector(playerPossible);
    std::vector<int> evaluations = {};

    // If there's only one possible move to make, make that move. Otherwise, evaluate all possible moves.
    if(pChildren.size() == 1) {
        Logger::logComment("Only one move to make, " + OutputHandler::getMoveOutput(playerColor, pChildren.at(0), false) + ", selecting that!");
        return pChildren.at(0);
    }

    std::cout << "C All possible moves: ";

    for(int child : pChildren) {
        int evaluation = minimax(playerDisks | (1LL << child), opponentDisks, 1, maxDepth, INT32_MIN, INT32_MAX, true);
        evaluations.push_back(evaluation);
    }

    // Iterate through child/evaluation score pairs and select best.
    int bestEval = INT32_MIN;
    int bestPos = INT32_MIN;

    for(int i = 0; i < evaluations.size(); i++) {
        int curEval = evaluations.at(i);
        int matchingChild = pChildren.at(i);

        if(curEval > bestEval) {
            bestEval = curEval;
            bestPos = matchingChild;
        }

        std::cout << "[" << OutputHandler::getMoveOutput(playerColor, matchingChild, false) << " | score = " << curEval << "] ";
    }

    std::cout << std::endl;
    Logger::logComment("Evaluated best position: " + OutputHandler::getMoveOutput(playerColor, bestPos, false) + " with score of " + std::to_string(bestEval));

    return bestPos;
}

std::vector<int> OthelloGameBoard::getMovesAsVector(uint64_t state) {
    auto vector = std::vector<int>();
    for(int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if((mask & state) != 0) {
            vector.push_back(i);
        }
    }

    return vector;
}
