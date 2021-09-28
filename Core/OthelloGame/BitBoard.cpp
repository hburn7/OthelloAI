//
// Created by Harry Burnett on 8/29/21.
//

#include <algorithm>
#include "BitBoard.h"
#include "OthelloGameBoard.h"

#define BLACK_BITS 0x0000000810000000
#define WHITE_BITS 0x0000001008000000

BitBoard::BitBoard(OthelloColor color) {
    this->color = color;

    switch(color) {
        case Black: {
            this->bits = BLACK_BITS;
            break;
        }
        case White: {
            this->bits = WHITE_BITS;
            break;
        }
    }
}

BitBoard::BitBoard(OthelloColor color, uint64_t bits) {
    this->color = color;
    this->bits = bits;
}

BitBoard::BitBoard(const BitBoard &oldBoard) {
    this->color = oldBoard.color;
    this->bits = oldBoard.bits;
}

BitBoard::BitBoard(const BitBoard &oldBoard, uint64_t bits) {
    this->color = oldBoard.color;
    this->bits = bits;
}

OthelloColor BitBoard::getColor() {
    return this->color;
}

std::array<std::array<int, 8>, 8> BitBoard::arrayRepresentation() {
    return OthelloGameBoard::binaryToArray(this->getBits());
}

uint64_t BitBoard::getBits() {
    return this->bits;
}

BitBoard BitBoard::setCellState(int pos) {
    uint64_t combinedBits = 1LL << pos | this->getBits();
    this->bits = combinedBits;

    return BitBoard(this->getColor(), this->getBits());
}

bool BitBoard::getCellState(int pos) {
    uint64_t mask = 1LL << pos;
    return ((this->getBits() & mask) != 0);
}

int BitBoard::getCellCount() {
    int count = 0;
    uint64_t bits = this->getBits();

    while(bits != 0) {
        bits &= bits - 1;
        count++;
    }

    return count;
}

void BitBoard::setBits(uint64_t bits) {
    this->bits = bits;
}
