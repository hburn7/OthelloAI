//
// Created by Harry Burnett on 8/29/21.
//

#include "OthelloGameBoard.h"

#define DIRECTION_COUNT 8
#define UNIVERSE 0xffffffffffffffffULL

#define CORNER_MASK 0x8100000000000081
#define CORNER_ADJACENT_MASK 0x42C300000000C342

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

// Assigns weight to every position on the board.
const std::array<int, 64> WEIGHT_MAP = {
        50, -20, 11, 8, 8, 11, -20, 50,
        -20, -35, -4, 1, 1, -4, -35, -20,
        11, -4, 2, 2, 2, 2, -4, 11,
        8, 1, 2, 0, 0, 2, 1, 8,
        8, 1, 2, 0, 0, 2, 1, 8,
        11, -4, 2, 2, 2, 2, -4, 11,
        -20, -35, -4, 1, 1, -4, -35, -20,
        50, -20, 11, 8, 8, 11, -20, 50
};

const std::map<int, std::array<int, 3>> STABILITY_IGNORES = {
        { 0, std::array<int, 3> { 1, 8, 9 } },
        { 7, std::array<int, 3> { 6, 14, 15 }, },
        { 56, std::array<int, 3> { 57, 48, 49 }, },
        { 62, std::array<int, 3> { 62, 55, 54 }, }
};

OthelloGameBoard::OthelloGameBoard(Config cfg, int playerColor, BitBoard player, BitBoard opponent) :
        m_cfg(cfg), m_playerColor(playerColor), m_playerBoard(player), m_opponentBoard(opponent) {}

OthelloGameBoard::OthelloGameBoard(const OthelloGameBoard &gameBoard) :
        m_cfg(gameBoard.getCfg()), m_playerColor(gameBoard.m_playerColor), m_playerBoard(gameBoard.m_playerBoard),
        m_opponentBoard(gameBoard.m_opponentBoard) {}

void OthelloGameBoard::drawBoard() {
    drawBoard(*this);
}

void OthelloGameBoard::drawBoard(OthelloGameBoard gameBoard) {
    Logger::logComment("    A B C D E F G H");
    Logger::logComment("    * * * * * * * *");

    BitBoard blackBoard = gameBoard.getForColor(BLACK);
    BitBoard whiteBoard = gameBoard.getForColor(WHITE);

    for (int i = 63; i >= 0; i--) {
        if (i % 8 == 7) {
            // We need to use manual prints in this specific case.
            std::cout << "C " << (-(i / 8) + 8) << " * ";
        }

        if (blackBoard.getCellState(i)) {
            Logger::logComment("B ", false);
        } else if (whiteBoard.getCellState(i)) {
            Logger::logComment("W ", false);
        } else {
            Logger::logComment("- ", false);
        }

        if (i % 8 == 0) {
            std::cout << std::endl;
        }
    }
}

BitBoard OthelloGameBoard::getPlayer() {
    return this->m_playerBoard;
}

BitBoard OthelloGameBoard::getOpponent() {
    return this->m_opponentBoard;
}

const Config &OthelloGameBoard::getCfg() const {
    return this->m_cfg;
}

void OthelloGameBoard::applyMove(BitBoard board, Move move) {
    board.applyIsolatedMove(move);

    // Update board internally.
    this->setForColor(board);
    this->lineCap(board, move);
}

BitBoard OthelloGameBoard::getBoard(int color) {
    switch (color) {
        case BLACK:
            return this->m_playerBoard;
        case WHITE:
            return this->m_opponentBoard;
        default:
            throw std::exception();
    }
}

bool OthelloGameBoard::isGameComplete() {
    return this->isGameComplete(*this);
}

bool OthelloGameBoard::isGameComplete(OthelloGameBoard gameBoard) {
    uint64_t playerDisks = gameBoard.getPlayer().getBits();
    uint64_t opponentDisks = gameBoard.getOpponent().getBits();

    auto agentLegal = this->generateMoveMask(playerDisks, opponentDisks);
    auto oppLegal = this->generateMoveMask(opponentDisks, playerDisks);

    return ((agentLegal == 0 && oppLegal == 0) || (playerDisks | opponentDisks) == UNIVERSE);
}

int OthelloGameBoard::countPieces(int color) {
    BitBoard toCount = color == this->getPlayerColor() ? this->getPlayer() : this->getOpponent();
    auto bitset = std::bitset<64>(toCount.getBits());
    return bitset.count();
}

int OthelloGameBoard::countBits(uint64_t bits) {
    std::bitset<64> bitset = std::bitset<64>(bits);
    return bitset.count();
}

// Credit to Dr. Mec for this algorithm.
uint64_t OthelloGameBoard::generateMoveMask(uint64_t playerDisks, uint64_t oppDisks) {
    // Returns move mask
    uint64_t emptyMask = ~playerDisks & ~oppDisks;
    uint64_t holdMask, dirMoveMask, moveMask = 0;

    for (int i = 0; i < DIRECTION_COUNT; i++) {
        // Finds opponent disks that are adjacent to player disks
        // in the current direction.
        holdMask = playerDisks;

        if (DIR_INCREMENTS[i] > 0) {
            holdMask = (holdMask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
        } else {
            holdMask = (holdMask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
        }
        holdMask = holdMask & oppDisks;

        // Find strings of 1 to 6 opponent disks that are adjacent
        // to the player in the current direction. Previous statement
        // has found the first opponent disk.
        for (int j = 0; ((j < 6) & (holdMask != 0LL)); j++) {
            if (DIR_INCREMENTS[i] > 0) {
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

void OthelloGameBoard::lineCap(BitBoard board, Move move) {
    // Move is already applied when running this function.
    BitBoard opp = this->getForColor(-board.getColor());

    uint64_t selfBits = board.getBits();
    uint64_t oppBits = opp.getBits();

    uint64_t mask = 1LL << move.getPos();
    uint64_t f_fin = 0LL;

    uint64_t possibility;

    for (int i = 0; i < DIRECTION_COUNT; i++) {
        uint64_t to_change = 0LL;
        uint64_t search;

        if (DIR_INCREMENTS[i] > 0) {
            search = (mask << DIR_INCREMENTS[i]) & DIR_MASKS[i];
        } else {
            search = (mask >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
        }

        possibility = oppBits & search;

        // Keep moving forward, identifying bits to flip
        // If we are in this loop, there's an opponent piece here.

        while (possibility != 0LL) {
            to_change |= possibility;
            if (DIR_INCREMENTS[i] > 0) {
                search = (search << DIR_INCREMENTS[i]) & DIR_MASKS[i];
            } else {
                search = (search >> -DIR_INCREMENTS[i]) & DIR_MASKS[i];
            }

            if ((selfBits & search) != 0) {
                f_fin |= to_change;
                break;
            }

            possibility = oppBits & search;
        }
    }

    selfBits |= f_fin;
    oppBits = (~f_fin) & oppBits;

    board.setBits(selfBits);
    opp.setBits(oppBits);

    this->setForColor(board);
    this->setForColor(opp);
}

// Helper to evaluate
bool OthelloGameBoard::ignoreAdjacents(int i) {
    auto matchingAdjacents = STABILITY_IGNORES.find(i);
    if(matchingAdjacents == STABILITY_IGNORES.end()) {
        return false;
    }

    return true;
}

double OthelloGameBoard::getSumWeight(int p_amt, int o_amt) {
    if(p_amt + o_amt == 0) {
        return 0;
    }

    int sum = p_amt + o_amt;
    int diff = p_amt - o_amt;

    double w = 100.0 * diff / sum;
    if(sum < 0) {
        return -std::abs(w);
    }

    return w;
}

int OthelloGameBoard::evaluate() {
    BitBoard pBoard = this->getPlayer();
    BitBoard oBoard = this->getOpponent();

    double wParity, wCorners, wAdjCorners, wMobility, wStability;

    int pCount = pBoard.getCellCount();
    int oCount = oBoard.getCellCount();

    uint64_t pMovesPossible = this->generateMoveMask(pBoard.getBits(), oBoard.getBits());
    uint64_t oMovesPossible = this->generateMoveMask(oBoard.getBits(), pBoard.getBits());

    // Individual position weight
    int pPosWeight = 0;
    int oPosWeight = 0;

    int pCorners = 0;
    int oCorners = 0;

    // Apply high negative weight to disks residing in corner-flankable spaces.
    int pAdjCorners = 0;
    int oAdjCorners = 0;

    uint64_t playerCornerMask = pBoard.getBits() & CORNER_MASK;
    uint64_t oppCornerMask = oBoard.getBits() & CORNER_MASK;

    uint64_t playerAdjCornerMask = pBoard.getBits() & CORNER_ADJACENT_MASK;
    uint64_t oppAdjCornerMask = oBoard.getBits() & CORNER_ADJACENT_MASK;

    auto pCornerPos = std::vector<int>{};
    auto oCornerPos = std::vector<int>{};

    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        int weight = WEIGHT_MAP[i];

        // Corners
        if ((playerCornerMask & mask) != 0) {
            pCorners++;
            pCornerPos.push_back(i);
        } else if ((oppCornerMask & mask) != 0) {
            oCorners++;
            oCornerPos.push_back(i);
        }

        // Adjacent corners
        if ((playerAdjCornerMask & mask) != 0) {
            pAdjCorners++;
        } else if ((oppAdjCornerMask & mask) != 0) {
            oAdjCorners++;
        }

        // Apply pos weights
        if ((pMovesPossible & mask) != 0) {
            pPosWeight += weight;
        } else if ((oMovesPossible & mask) != 0) {
            oPosWeight += weight;
        }
    }

    for(int i = 0; i < pCornerPos.size(); i++) {
        auto matchingAdjacents = STABILITY_IGNORES.find(pCornerPos.at(i));

        if(matchingAdjacents != STABILITY_IGNORES.end()) {
            std::array<int, 3> adjacent = matchingAdjacents->second;
            for(int j = 0; j < adjacent.size(); j++) {
                int known = adjacent.at(j);
                if(known == i) {
                    pPosWeight -= WEIGHT_MAP[i];
                    pAdjCorners -= 1;
                }
            }
        }
    }

//    for(int i = 0; i < oCornerPos.size(); i++) {
//        auto matchingAdjacents = STABILITY_IGNORES.find(oCornerPos.at(i));
//
//        if(matchingAdjacents != STABILITY_IGNORES.end()) {
//            std::array<int, 3> adjacent = matchingAdjacents->second;
//            for(int j = 0; j < adjacent.size(); j++) {
//                int known = adjacent.at(j);
//                if(known == i) {
//                    oPosWeight -= WEIGHT_MAP[i];
//                    oAdjCorners -= 1;
//                }
//            }
//        }
//    }

    wStability = getSumWeight(pPosWeight, oPosWeight);
    wParity = getSumWeight(pCount, oCount);
    wCorners = getSumWeight(pCorners, oCorners);
    wAdjCorners = -getSumWeight(pAdjCorners, oAdjCorners);
    wMobility = getSumWeight(countBits(pMovesPossible), countBits(oMovesPossible));

    int fCorners = 200;
    int fAdjacent = 20;
    int fMobility = 28;
    int fParity = 8;
    int fStability = 45;

    if(oMovesPossible == 0) {
        fMobility = 500;
    }

    int sumMoves = pCount + oCount;
    if(sumMoves >= 50) {
        fParity = 35;
        fMobility = 30;
    }
    if(sumMoves >= 58) {
        fParity = 200;
        fMobility = 10;
        fStability = 5;
    }

    int score = (int) (fCorners * wCorners) + /*(fAdjacent * wAdjCorners) +*/ (fMobility * wMobility) +
                (fParity * wParity) + (fStability * wStability);

    // End game. Return below for confirmed win / loss.
    if(sumMoves == 64) {
        if(pCount < oCount) {
            return INT32_MIN;
        } else {
            return INT32_MAX;
        }
    }

    return score;
}

std::pair<int, int> OthelloGameBoard::alphaBeta(OthelloGameBoard gameBoard, int player, int depth, int maxDepth,
                                 uint64_t stopTime, int alpha, int beta, bool max) {
    if(depth >= maxDepth || gameBoard.isGameComplete()) {
        return { gameBoard.evaluate(), depth };
    }

    BitBoard pBoard = gameBoard.getForColor(player);
    BitBoard oBoard = gameBoard.getForColor(-player);

    auto moves = this->generateMovesAsPriorityQueue(pBoard, oBoard);

    if(max) {
        if(moves.empty()) {
            return { gameBoard.evaluate(), depth };
        }

        int maxEval = INT32_MIN;
        while(!moves.empty()) {
            auto newMaximizerBoard = BitBoard(pBoard);
            auto newBoard = OthelloGameBoard(gameBoard);
            newBoard.applyMove(newMaximizerBoard, moves.top());
            moves.pop();

            auto eval = alphaBeta(newBoard, -player, depth + 1, maxDepth, stopTime, alpha, beta, false);
            maxEval = std::max(maxEval, eval.first);
            alpha = std::max(alpha, eval.first);
            if(beta <= alpha) {
                break;
            }
        }
        return { maxEval, depth };
    } else {

        if(moves.empty()) {
            return { gameBoard.evaluate(), depth };
        }

        int minEval = INT32_MAX;
        while(!moves.empty()) {
            auto newPBoard = BitBoard(pBoard);
            auto newBoard = OthelloGameBoard(gameBoard);

            newBoard.applyMove(newPBoard, moves.top());
            moves.pop();

            auto eval = alphaBeta(newBoard, -player, depth + 1, maxDepth, stopTime, alpha, beta, true);
            minEval = std::min(minEval, eval.first);
            beta = std::min(beta, eval.first);
            if(beta <= alpha) {
                break;
            }
        }
        return { minEval, depth };
    }
}

// TODO: Convert random param to sep function 'getRandomMove()'
Move OthelloGameBoard::selectMove(int playerColor, bool random) {
    BitBoard primary = this->getForColor(playerColor);
    BitBoard opponent = this->getForColor(-playerColor);

    // Calculates the time to iteratively search 1 move out of the child list.
    // This way, all children are given equal search time.

    auto possibleMoves = this->generateMovesAsPriorityQueue(primary, opponent);

    if(possibleMoves.empty()) {
        return Move();
    }

    if(random) {
        return possibleMoves.top();
    }

    // Moves generated, but only for the sake of counting them. Moves are generated again inside alphaBeta.
    int staticTime = (int) ((this->getCfg().getMoveTime() / (double) possibleMoves.size()) * 1000);

    Logger::logComment("Allowing " + std::to_string(staticTime) + "ms for next evaluations.");

    Move bestMove = possibleMoves.top();
    ulong size = possibleMoves.size();
    int counter = 1;
    while(!possibleMoves.empty()) {
        Move cur = possibleMoves.top();
        possibleMoves.pop();

        auto newPrimary = BitBoard(primary);
        auto newBoard = OthelloGameBoard(*this);
        newBoard.applyMove(newPrimary, cur);

        int maxDepth = 10;
        auto eval = this->alphaBeta(newBoard, -playerColor, 1, maxDepth, 0, INT32_MIN, INT32_MAX, true);
        cur.setValue(eval.first);

        std::string curMoveReadable = OutputHandler::getMoveOutput(primary.getColor(), cur, false);
        char *cstr = new char[curMoveReadable.length() + 1];
        strcpy(cstr, curMoveReadable.c_str());

        printf("C Evaluated move [%s] (%d / %lu) to depth %d with score %d\n", cstr, counter, size, maxDepth, cur.getValue());

        if(cur.getValue() > bestMove.getValue()) {
            bestMove = cur;
        }

        counter++;
    }

    return bestMove;
}

std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>> OthelloGameBoard::getMovesAsPriorityQueue(uint64_t state) {
    std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>> pQueue;
    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if ((mask & state) != 0) {
            int weight = WEIGHT_MAP[i];
            pQueue.push(Move(i, weight));
        }
    }

    return pQueue;
}

uint64_t OthelloGameBoard::getCurrentSysTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
}

BitBoard OthelloGameBoard::getForColor(int color) {
    return color == m_playerColor ? this->getPlayer() : this->getOpponent();
}

int OthelloGameBoard::getPlayerColor() {
    return this->m_playerColor;
}

int OthelloGameBoard::getOpponentColor() {
    return this->m_playerColor == WHITE ? BLACK : WHITE;
}

void OthelloGameBoard::setForColor(BitBoard board) {
    auto playerBoard = this->getPlayer();
    if (board.getColor() == playerBoard.getColor()) {
        this->m_playerBoard = BitBoard(board);
    } else {
        this->m_opponentBoard = BitBoard(board);
    }
}

std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>>
OthelloGameBoard::generateMovesAsPriorityQueue(BitBoard player, BitBoard opponent) {
    auto queue = std::priority_queue<Move, std::vector<Move>, std::less<std::vector<Move>::value_type>>();
    uint64_t possibilities = this->generateMoveMask(player.getBits(), opponent.getBits());
    return this->getMovesAsPriorityQueue(possibilities);
}