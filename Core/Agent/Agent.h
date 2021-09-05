//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_AGENT_H
#define OTHELLOPROJECT_CPP_AGENT_H

#include <vector>

#include "../OthelloGame/BitBoard.h"
#include "../OthelloGame/OthelloColor.h"
#include "../OthelloGame/OthelloGameBoard.h"

/**
 * The Computer is responsible for automatically playing a legal game of Othello with an
 * AI component. The AI component ranks all possible moves by simulating the outcomes of each applyMove,
 * all the way to the end-game, then picks the best one.
 *
 * Until the AI component is implemented, the Computer will select moves at random.
 */
class Agent {
public:
    /**
     * Initializes the agent with a given color to play as.
     * @param color The color to play as for this game.
     */
    Agent(OthelloColor color, BitBoard self, BitBoard opponent);
    std::array<int, 2> getMove();

    // Getters and setters

    OthelloColor getColor() const;
    void setColor(OthelloColor color);

    const BitBoard &getSelf() const;
    void setSelf(const BitBoard &self);

    const BitBoard &getOpponent() const;
    void setOpponent(const BitBoard &opponent);
private:
    OthelloColor color;
    BitBoard self;
    BitBoard opponent;

    /**
     * Searches through the black and white bitboards to collect all possible moves into a vector.
     * @param black Black bitboard
     * @param white White bitboard
     * @return A std::vector<std::array<int, 2>> of all possible coordinates at which the bot can applyMove to.
     */
    std::vector<std::array<int, 2>> allPossibleMoves();
    /**
     * Selects a applyMove at random to play.
     * TODO: Implement AI component that searches and ranks all possible outcomes of all possible moves.
     * @param allPossibleMoves All possible moves in play this turn.
     * @return A coordinate pair at which the Computer will applyMove to.
     */
    std::array<int, 2> selectMove(std::vector<std::array<int, 2>> allPossibleMoves);
};


#endif //OTHELLOPROJECT_CPP_AGENT_H
