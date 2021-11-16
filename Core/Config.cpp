//
// Created by stage on 10/21/21.
//

#include "Config.h"

Config::Config(bool interactive, int game_time) : m_move_time(game_time / 30.0), m_interactive(interactive) {}

double Config::getMoveTime() const {
    return m_move_time;
}

bool Config::isInteractive() const {
    return m_interactive;
}