//
// Created by Harry Burnett on 8/29/21.
//

#ifndef OTHELLOPROJECT_CPP_DIRECTIVE_H
#define OTHELLOPROJECT_CPP_DIRECTIVE_H

enum Directive {
    InitializeBlack,
    InitializeWhite,
    MoveSelf, // Self is always in reference to this program's agent (bot).
    MoveOpponent,
    PassBlack,
    PassWhite,
    Comment,
    EndGame,
    Invalid
};

#endif //OTHELLOPROJECT_CPP_DIRECTIVE_H
