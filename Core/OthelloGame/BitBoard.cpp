//
// Created by Harry Burnett on 8/29/21.
//

#include "BitBoard.h"
#include "Color.h"

#define BLACK_BITS 0x0000000810000000
#define WHITE_BITS 0x0000001008000000

BitBoard::BitBoard(int color) {
    this->color = color;
    this->bits = color == BLACK ? BLACK_BITS : WHITE_BITS;
}

BitBoard::BitBoard(const BitBoard &oldBoard) {
    this->color = oldBoard.color;
    this->bits = oldBoard.bits;
}

int BitBoard::getColor() {
    return this->color;
}

uint64_t BitBoard::getBits() {
    return this->bits;
}

void BitBoard::applyIsolatedMove(Move move) {
    this->setBits(this->getBits() | (1LL << move.getPos()));
}

bool BitBoard::getCellState(int pos) {
    uint64_t mask = 1LL << pos;
    return ((this->getBits() & mask) != 0);
}

int BitBoard::getCellCount() {
    int count = 0;
    uint64_t curBits = this->getBits();

    while(curBits != 0) {
        curBits &= curBits - 1;
        count++;
    }

    return count;
}

void BitBoard::setBits(uint64_t bits) {
    this->bits = bits;
}
