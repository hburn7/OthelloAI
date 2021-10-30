//
// Created by stage on 10/21/21.
//

#include "Config.h"

Config::Config(bool interactive, int game_time) : m_time_remaining(game_time), m_interactive(interactive) {
    // There are 30 moves to make in a game. Allocate equal time for each.
    this->m_move_time = game_time / 30.0;
}

int Config::getTimeRemaining() const {
    return m_time_remaining;
}

void Config::setTimeRemaining(int mTimeRemaining) {
    m_time_remaining = mTimeRemaining;
}

double Config::getMoveTime() const {
    return m_move_time;
}

bool Config::isInteractive() const {
    return m_interactive;
}