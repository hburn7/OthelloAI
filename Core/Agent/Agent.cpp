//
// Created by Harry Burnett on 9/25/21.
//

#include "Agent.h"

Agent::Agent(OthelloColor c) : m_board(BitBoard(c)) {}

int Agent::selectMove(uint64_t possibilities) {
    // For now, select an arbitrary move.
    for(int i = 0; i < 64; i++) {
        uint64_t mask = 1LL << i;
        if((possibilities & mask) != 0) {
            return i;
        }
    }

    return -1; // Pass?
}

void Agent::setBitBoard(BitBoard board) {
    this->m_board = board;
}

BitBoard Agent::getBitBoard() {
    return this->m_board;
}
