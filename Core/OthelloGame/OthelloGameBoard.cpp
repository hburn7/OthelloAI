//
// Created by Harry Burnett on 8/29/21.
//

#include "OthelloGameBoard.h"

#define DIRECTION_COUNT 8
#define UNIVERSE 0xffffffffffffffffULL

#define CORNER_MASK 0x8100000000000081
#define CORNER_ADJACENT_MASK 0x42C3000000C342

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

// Assigns weights to every position on the board.
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

        uint64_t mask = 1LL << i;
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

int OthelloGameBoard::evaluate(OthelloGameBoard gameBoard) {
    uint64_t playerDisks = gameBoard.getPlayer().getBits();
    uint64_t opponentDisks = gameBoard.getOpponent().getBits();

    double wParity = 0, wCorners = 0, wAdjCorners = 0, wMobility = 0, wStability = 0;

    int countPlayer = gameBoard.getPlayer().getCellCount();
    int countOpponent = gameBoard.getOpponent().getCellCount();

    uint64_t playerPossibleMoves = this->generateMoveMask(playerDisks, opponentDisks);
    uint64_t opponentPossibleMoves = this->generateMoveMask(opponentDisks, playerDisks);

    // Individual position weight
    int playerPosWeight = 0;
    int oppPosWeight = 0;

    int playerCornerCount = 0;
    int oppCornerCount = 0;

    // Apply high negative weight to disks residing in corner-flankable spaces.
    int playerAdjCornerCount = 0;
    int oppAdjCornerCount = 0;

    uint64_t playerCornerMask = playerDisks & CORNER_MASK;
    uint64_t oppCornerMask = opponentDisks & CORNER_MASK;

    uint64_t playerAdjCornerMask = playerDisks & CORNER_ADJACENT_MASK;
    uint64_t oppAdjCornerMask = opponentDisks & CORNER_ADJACENT_MASK;

    for (int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        int curWeight = WEIGHT_MAP[i];

        // Apply pos weights
        if ((playerPossibleMoves & mask) != 0) {
            playerPosWeight += curWeight;
        } else if ((opponentPossibleMoves & mask) != 0) {
            oppPosWeight += curWeight;
        }

        // Count corners
        if ((playerCornerMask & mask) != 0) {
            playerCornerCount++;
        } else if ((oppCornerMask & mask) != 0) {
            oppCornerCount++;
        }

        // Count areas adjacent to corners
        if ((playerAdjCornerMask & mask) != 0) {
            playerAdjCornerCount++;
        } else if ((oppAdjCornerMask & mask) != 0) {
            oppAdjCornerCount++;
        }
    }

    // Stability
    int weightDiff = playerPosWeight - oppPosWeight;
    wStability = 100.0 * weightDiff / (playerPosWeight + oppPosWeight);
    if (weightDiff < 0) {
        wStability = -std::abs(wStability);
    }

    // Coin parity
    int countDiff = countPlayer - countOpponent;
    wParity = 100.0 * countDiff / (countPlayer + countOpponent);
    if (countDiff < 0) {
        wParity = -std::abs(wParity);
    }

    // Corners
    int cornerDiff = playerCornerCount - oppCornerCount;
    int sumCorners = playerCornerCount + oppCornerCount;

    if (sumCorners == 0) {
        // Prevents div / 0 errors.
        wCorners = 0;
    } else {
        wCorners = 100.0 * cornerDiff / sumCorners;
        if (cornerDiff < 0) {
            wCorners = -std::abs(wCorners);
        }
    }

    // Adjacent Corners
    int adjCornerDiff = playerAdjCornerCount - oppAdjCornerCount;
    int sumAdj = playerAdjCornerCount + oppAdjCornerCount;

    if (sumAdj == 0) {
        wAdjCorners = 0;
    } else {
        wAdjCorners = 100.0 * adjCornerDiff / sumAdj;
        if (cornerDiff < 0) {
            wAdjCorners = -std::abs(wAdjCorners);
        }
    }

    // Mobility
    int countMovesPlayer = this->countBits(playerPossibleMoves);
    int countMovesOpponent = this->countBits(opponentPossibleMoves);

    int moveDiff = countMovesPlayer - countMovesOpponent;
    int sumAllMoves = countMovesPlayer + countMovesOpponent;
    wMobility = 100.0 * moveDiff / sumAllMoves;
    if (moveDiff < 0) {
        wMobility = -std::abs(wMobility);
    }

    bool forcePass = false;
    if (countMovesOpponent == 0) {
        // Amplify by a high value to showcase forcing pass onto opponent or being forced to pass.
        // Amplification will result in a stronger positive or stronger negative number per the formula above.
        forcePass = true;
    }

    int factorCorners = 90;
    int factorAdjCorners = 10;
    int factorMobility = 15;
    int factorParity = 5;
    int factorStability = 45;

    int score = (int) (factorCorners * wCorners) + (factorAdjCorners * wAdjCorners) + (factorMobility * wMobility) +
                (factorParity * wParity) + (factorStability * wStability);
    if (forcePass) {
        score += 1000 * sumAllMoves; // Higher value given to end-game force passes.
    }

    if (sumAllMoves == 64) {
        // Game over
        if (wParity < 0) {
            score = -1; // We lost
        } else {
            score = 1; // We won
        }
    }

    return score;
}

std::pair<int, int> OthelloGameBoard::alphaBeta(OthelloGameBoard gameBoard, int player, int depth, int maxDepth,
                                 uint64_t stopTime, int alpha, int beta, bool max) {
    if(depth >= maxDepth) {
        return std::pair<int, int>(this->evaluate(gameBoard), depth);
    }

    BitBoard maximizerBoard = gameBoard.getForColor(player);
    BitBoard minimizerBoard = gameBoard.getForColor(-player);

    auto moves = this->generateMovesAsPriorityQueue(maximizerBoard, minimizerBoard);

    if(max) {
        if(moves.empty()) {
            return std::pair<int, int>(-9999999, depth);
        }

        int maxEval = INT32_MIN;
        while(!moves.empty()) {
            OthelloGameBoard newBoard = OthelloGameBoard(gameBoard);
            newBoard.applyMove(maximizerBoard, moves.top());
            moves.pop();

            auto eval = alphaBeta(gameBoard, -player, depth + 1, maxDepth, stopTime, alpha, beta, false);
            maxEval = std::max(maxEval, eval.first);
            alpha = std::max(alpha, eval.first);
            if(beta <= alpha) {
                break;
            }
        }
        return std::pair<int, int>(maxEval, depth);
    } else {

        if(moves.empty()) {
            return std::pair<int, int>(-9999999, depth);
        }

        int minEval = INT32_MAX;
        while(!moves.empty()) {
            OthelloGameBoard newBoard = OthelloGameBoard(gameBoard);
            newBoard.applyMove(maximizerBoard, moves.top());
            moves.pop();

            auto eval = alphaBeta(gameBoard, -player, depth + 1, maxDepth, stopTime, alpha, beta, true);
            minEval = std::min(minEval, eval.first);
            beta = std::min(beta, eval.first);
            if(beta <= alpha) {
                break;
            }
        }
        return std::pair<int, int>(minEval, depth);
    }
}

// TODO: Convert random param to sep function 'getRandomMove()'
Move OthelloGameBoard::selectMove(int playerColor, bool random) {
    BitBoard primary = this->getForColor(playerColor);
    BitBoard opponent = this->getForColor(-playerColor);

    // Calculates the time to iteratively search 1 move out of the child list.
    // This way, all children are given equal search time.

    auto possibleMoves = this->generateMovesAsPriorityQueue(primary, opponent);

    if(random) {
        return possibleMoves.top();
    }

    // Moves generated, but only for the sake of counting them. Moves are generated again inside alphaBeta.
    int staticTime = (int) ((this->getCfg().getMoveTime() / (double) possibleMoves.size()) * 1000);

    Logger::logComment("Allowing " + std::to_string(staticTime) + "ms for next evaluations.");

    Move bestMove = possibleMoves.top();
    int size = possibleMoves.size();
    int counter = 1;
    while(!possibleMoves.empty()) {
        Move next = possibleMoves.top();
        possibleMoves.pop();

        BitBoard newPrimary = BitBoard(primary);
        OthelloGameBoard newBoard = OthelloGameBoard(*this);
        newBoard.applyMove(newPrimary, next);

        int maxDepth = 2;
        uint64_t endTime = this->getCurrentSysTime() + staticTime;

        Move placeholder = Move();

        for(; endTime - this->getCurrentSysTime() > 0; maxDepth += 2) {
            auto evaluation = alphaBeta(newBoard, playerColor, 1, maxDepth, endTime, INT32_MIN, INT32_MAX, true);

            Move cur = Move(next.getPos(), evaluation.first);

            if(placeholder.getPos() != cur.getPos()) {
                placeholder = cur;
                std::string curMoveReadable = OutputHandler::getMoveOutput(primary.getColor(), cur, false);
                char *cstr = new char[curMoveReadable.length() + 1];
                strcpy(cstr, curMoveReadable.c_str());

                printf("C Evaluated move [%s] (%d / %d) to depth %d with score %d\n", cstr, counter, size, evaluation.second, cur.getValue());

                counter++;
            }

            if(cur.getValue() > bestMove.getValue()) {
                bestMove = cur;
            }
        }
    }
    return bestMove;

//    int sumDisks = this->countBits(primary.getBits() | opponent.getBits());
//
//    auto pChildren = this->generateMovesAsPriorityQueue(primary, opponent);
//
//    if (pChildren.size() == 0) {
//        Logger::logComment("Passing, no moves to make for primary.");
//        return -1; // Pass, cannot make any moves
//    }
//
//    // If the agent is a random agent (testing purposes only)
//    if (random && pChildren.size() > 0) {
//        return pChildren.top().second;
//    }
//
//    // If there's only one possible move to make, make that move. Otherwise, evaluate all possible moves.
//    if (pChildren.size() == 1) {
//        int move = pChildren.top().second;
//        pChildren.pop();
//
//        Logger::logComment("Only one move to make, " + OutputHandler::getMoveOutput(playerColor, move, false) +
//                           ", selecting that!");
//        return move;
//    }
//
//

//
//    std::priority_queue<std::pair<int, int>> evaluations = {};
//    int childrenSize = pChildren.size();
//
//    int counter = 1;
//    OthelloGameBoard copy = OthelloGameBoard(gameBoard);
//
//    while (!pChildren.empty()) {
//        int timeRemaining = staticTime;
//        uint64_t executionTime;
//        uint64_t stopTime;
//
//        auto nextBest = pChildren.top();
//        pChildren.pop();
//
//        copy.applyMove(primary, nextBest.second);
//
//        int curEval;
//        int prevEval = INT32_MIN;
//        int curMaxDepth = 2;
//
//        // CurEval is discarded as the alphaBeta evaluation at the max depth was interrupted due to time and not completed properly.
//        for (; timeRemaining > 0 && curMaxDepth <= 64 - sumDisks; curMaxDepth += 2) {
//            // stopTime tells alphaBeta at what point on the system clock to abort evaluating.
//            // This will result in an incomplete search. By default, the last search returned
//            // by alphaBeta is always ignored. This is because we assume all searches are interrupted by time.
//            // End-game searches are never ignored because they complete faster than the time limit.
//
//            executionTime = this->getCurrentSysTime();
//            stopTime = executionTime + timeRemaining;
//
//            if (curMaxDepth > 2 && curEval != INT32_MIN) {
//                prevEval = curEval;
//            }
//
//            curEval = minimax(copy, 2, curMaxDepth, stopTime, INT32_MIN,
//                              INT32_MAX, true, agentPlay);
//
//            timeRemaining -= this->getCurrentSysTime() - executionTime;
//        }
//
//        if(curEval == INT32_MIN) {
//            curEval = prevEval;
//        }
//
//        evaluations.push(std::pair<int, int>(curEval, nextBest.second));
//
//        std::string curMoveReadable = OutputHandler::getMoveOutput(primary.getColor(), curEval, false);
//        char *cstr = new char[curMoveReadable.length() + 1];
//        strcpy(cstr, curMoveReadable.c_str());
//
//        printf("C Evaluated move [%s] (%d / %d) to depth %d with score %d\n", cstr, counter, childrenSize, curMaxDepth, curEval);
//
//        counter++;
//    }
//
//    // Iterate through child/evaluation score pairs and select best.
//    auto retBest = evaluations.top();
//
//    std::cout << "C All possible moves: ";
//    while (!evaluations.empty()) {
//        auto best = evaluations.top();
//        evaluations.pop();
//
//        std::cout << "[" << OutputHandler::getMoveOutput(playerColor, best.second, false) << " | score = " << best.first
//                  << "] ";
//    }
//
//    std::cout << std::endl;
//    Logger::logComment("Evaluated best position: " + OutputHandler::getMoveOutput(playerColor, retBest.second, false) +
//                       " with score of " + std::to_string(retBest.first));
//
//    return retBest.second;
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