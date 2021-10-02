//
// Created by Harry Burnett on 9/25/21.
//

#ifndef OTHELLOPROJECT_CPP_AGENT_H
#define OTHELLOPROJECT_CPP_AGENT_H

#include "../OthelloGame/OthelloColor.h"
#include "../OthelloGame/BitBoard.h"

class Agent {
public:
    /**
     * Creates a new agent with the given color and starting bits.
     */
    Agent(OthelloColor c);
    /**
     * Generates a move for the agent.
     * @return The position on the board, ranged 0-63 inclusive, to move to.
     */
    int selectMove(uint64_t possibilities);
private:
    BitBoard m_board;
};


#endif //OTHELLOPROJECT_CPP_AGENT_H
