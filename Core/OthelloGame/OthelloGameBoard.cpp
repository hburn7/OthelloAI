//
// Created by Harry Burnett on 8/29/21.
//

#include "OthelloGameBoard.h"

#define DIRECTION_COUNT 8
#define UNIVERSE 0xffffffffffffffffULL

#define CORNER_MASK 0x8100000000000081
#define CORNER_ADJACENT_MASK 0x42C3000000C342

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
        20, -7, 11, 8, 8, 11, -7, 20,
        -7, -12, -4, 1, 1, -4, -12, -7,
        11, -4, 2, 2, 2, 2, -4, 11,
        8, 1, 2, 0, 0, 2, 1, 8,
        8, 1, 2, 0, 0, 2, 1, 8,
        11, -4, 2, 2, 2, 2, -4, 11,
        -7, -12, -4, 1, 1, -4, -12, -7,
        20, -7, 11, 8, 8, 11, -7, 20
};

OthelloGameBoard::OthelloGameBoard(Config cfg, BitBoard black, BitBoard white) : m_black(black), m_white(white), m_cfg(cfg) {}

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

const Config &OthelloGameBoard::getCfg() const {
    return this->m_cfg;
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
    int wParity = 0, wCorners = 0, wMobility = 0, wPosWeight = 0, wStability = 0;

    int countPlayer = this->countBits(playerDisks);
    int countOpponent = this->countBits(opponentDisks);
    wParity = 100 * (countPlayer - countOpponent) / (countPlayer + countOpponent);

    int countTotal = countPlayer + countOpponent;

    uint64_t playerPossibleMoves = this->generateMoves(playerDisks, opponentDisks);
    uint64_t opponentPossibleMoves = this->generateMoves(opponentDisks, playerDisks);

    // Individual position weight, stability value

    int playerStableDisks = 0;
    int oppStableDisks = 0;

    // Stability
    if(playerStableDisks + oppStableDisks == 0) {
        wStability = 0;
    } else {
        wStability = 100 * (playerStableDisks - oppStableDisks) / (playerStableDisks + oppStableDisks);
    }

    // Coin parity
    wParity = 100 * (countPlayer - countOpponent) / (countPlayer + countOpponent + 1);
    if(countOpponent > countPlayer) {
        wParity = -wParity;
    }

    // Corners
    auto playerCornerBitset = std::bitset<64>(playerDisks & CORNER_MASK);
    auto opponentCornerBitset = std::bitset<64>(opponentDisks & CORNER_MASK);
    auto allCornersCaptured = playerCornerBitset.count() + opponentCornerBitset.count();

    if(allCornersCaptured == 0) {
        wCorners = 0;
    } else {
        wCorners = 100 * (playerCornerBitset.count() + opponentCornerBitset.count()) / (playerCornerBitset.count() + opponentCornerBitset.count());
    }

    // Mobility
    int countMovesPlayer = this->countBits(playerPossibleMoves);
    int countMovesOpponent = this->countBits(opponentPossibleMoves);
    int countAllMoves = countMovesPlayer + countMovesOpponent;

    if(countMovesOpponent == 0 && countAllMoves != 1) {
        wMobility = INT32_MAX;
    } else if (countMovesPlayer == 0 && countAllMoves != 1){
        wMobility = INT32_MIN;
    } else if(countAllMoves == 0) {
        wMobility = 0;
    } else {
        wMobility = 100 * (countMovesPlayer - countMovesOpponent) / (countMovesPlayer + countMovesOpponent);
    }

    int score = 100 * wCorners + 5 * wMobility + 25 * wParity + 25 * wStability;
    return score;
}

int OthelloGameBoard::minimax(int pos, uint64_t playerDisks, uint64_t opponentDisks, int depth,
                              uint64_t startTime, uint64_t timeRemaining, int alpha, int beta, bool maximizingPlayer) {
    uint64_t currentSysTime = getCurrentSysTime();

    timeRemaining -= (currentSysTime - startTime);

    // Todo: Time, iterative deepening
    if(timeRemaining > 5000 || this->isGameComplete(playerDisks, opponentDisks)) {
        int evaluation = this->evaluate(playerDisks, opponentDisks);
        return evaluation;
    }

    if(maximizingPlayer) {
        int maxEval = INT32_MIN;
        playerDisks |= (1LL << pos);

        uint64_t children = this->generateMoves(playerDisks, opponentDisks);
        auto childQueue = this->getMovesAsPriorityQueue(children);

        if(children == 0) {
            return this->evaluate(playerDisks, opponentDisks);
        }

        while(!childQueue.empty()) {
            auto curBest = childQueue.top();
            childQueue.pop();

            int eval = minimax(curBest.second, playerDisks, opponentDisks, depth + 1, startTime, timeRemaining, alpha, beta, !maximizingPlayer);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, maxEval);

            if(beta <= alpha) {
                break;
            }
        }

        return maxEval;
    } else {
        int minEval = INT32_MAX;
        opponentDisks |= (1LL << pos);

        uint64_t children = this->generateMoves(opponentDisks, playerDisks);
        auto childQueue = this->getMovesAsPriorityQueue(children);

        if(children == 0) {
            return this->evaluate(playerDisks, opponentDisks);
        }

        while(!childQueue.empty()) {
            auto curBest = childQueue.top();
            childQueue.pop();

            int eval = minimax(curBest.second, playerDisks, opponentDisks, depth + 1, startTime, timeRemaining, alpha, beta, !maximizingPlayer);
            minEval = std::min(minEval, eval);
            beta = std::min(minEval, beta);

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

    std::priority_queue<std::pair<int, int>> pChildren = this->getMovesAsPriorityQueue(playerPossible);
    std::priority_queue<std::pair<int, int>> evaluations = {};

    // If there's only one possible move to make, make that move. Otherwise, evaluate all possible moves.
    if(pChildren.size() == 1) {
        int move = pChildren.top().second;
        pChildren.pop();

        Logger::logComment("Only one move to make, " + OutputHandler::getMoveOutput(playerColor, move, false) + ", selecting that!");
        return move;
    }

    std::cout << "C All possible moves: ";

    uint64_t executionTime = this->getCurrentSysTime();
    uint64_t timeRemaining = this->getCfg().getMoveTime() / pChildren.size() * 1000;

    while(!pChildren.empty()) {
        auto nextBest = pChildren.top();
        pChildren.pop();

        int evaluation = minimax(nextBest.second, playerDisks, opponentDisks, 1, executionTime, timeRemaining, INT32_MIN, INT32_MAX, true);
        evaluations.push(std::pair<int, int>(evaluation, nextBest.second));
    }

    // Iterate through child/evaluation score pairs and select best.
    auto retBest = evaluations.top();

    while(!evaluations.empty()) {
        auto best = evaluations.top();
        evaluations.pop();

        std::cout << "[" << OutputHandler::getMoveOutput(playerColor, best.second, false) << " | score = " << best.first << "] ";
    }

    std::cout << std::endl;
    Logger::logComment("Evaluated best position: " + OutputHandler::getMoveOutput(playerColor, retBest.second, false) +
    " with score of " + std::to_string(retBest.first));

    return retBest.second;
}

std::priority_queue<std::pair<int, int>> OthelloGameBoard::getMovesAsPriorityQueue(uint64_t state) {
    auto pQueue = std::priority_queue<std::pair<int, int>>();
    for(int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if((mask & state) != 0) {
            int weight = WEIGHT_MAP[i];
            pQueue.push(std::pair<int, int>(weight, i));
        }
    }

    return pQueue;
}

long OthelloGameBoard::getCurrentSysTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
