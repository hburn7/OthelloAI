//
// Created by hburn7 on 10/31/21.
//

#include "Move.h"
#include <stdint.h>

Move::Move() : m_isPass(true), m_pos(-1), m_value(INT32_MIN) {}
Move::Move(int pos, int value) : m_isPass(false), m_pos(pos), m_value(value) {}

int Move::getPos() const {
    return m_pos;
}

int Move::getValue() const {
    return m_value;
}

void Move::setValue(int value) {
    this->m_value = value;
}

bool Move::isPass() {
    return this->m_isPass;
}

bool operator < (const Move &lhs, const Move &rhs) {
    return lhs.getValue() < rhs.getValue();
}
