//
// Created by Harry Burnett on 8/30/21.
//

#ifndef OTHELLOPROJECT_CPP_GAMESTATE_H
#define OTHELLOPROJECT_CPP_GAMESTATE_H

enum GameState {
    Player,
    Computer // Cannot use "Agent" here as it conflicts with Agent::Agent()..
};

static GameState swapState(GameState gs);

#endif //OTHELLOPROJECT_CPP_GAMESTATE_H
