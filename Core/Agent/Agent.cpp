//
// Created by Harry Burnett on 8/29/21.
//

#include "Agent.h"
#include "../Logger.h"

Agent::Agent(OthelloColor color, BitBoard self, BitBoard opponent) : color(color), self(self), opponent(opponent) {}

std::vector<std::array<int, 2>> Agent::allPossibleMoves() {
    // TODO: Implement
    Logger::logComment("Computer is determining all possible moves. (All possible moves)");
    Logger::logComment("Computer is ranking the moves. (All possible moves)");
    Logger::logComment("Computer found best possible applyMove. (All possible moves)");

    return std::vector<std::array<int, 2>>();
}

std::array<int, 2> Agent::selectMove(std::vector<std::array<int, 2>> allPossibleMoves) {
    int rIdx = std::rand() % allPossibleMoves.size();
    return allPossibleMoves[rIdx];
}

std::array<int, 2> Agent::getMove() {
    return std::array<int, 2>();
}

OthelloColor Agent::getColor() const {
    return color;
}

void Agent::setColor(OthelloColor color) {
    Agent::color = color;
}

const BitBoard &Agent::getSelf() const {
    return self;
}

void Agent::setSelf(const BitBoard &self) {
    Agent::self = self;
}

const BitBoard &Agent::getOpponent() const {
    return opponent;
}

void Agent::setOpponent(const BitBoard &opponent) {
    Agent::opponent = opponent;
}

long long shiftNorth(long long n) {

}