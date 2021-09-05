//
// Created by Harry Burnett on 8/30/21.
//

#include "GameState.h"

GameState swapState(GameState gs) {
    switch(gs) {
        case GameState::Player:
            return GameState::Computer;
        case GameState::Computer:
            return GameState::Player;
    }
}