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

    // Pass if no possible moves are available
    return -1;
}