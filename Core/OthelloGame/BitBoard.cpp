//
// Created by Harry Burnett on 8/29/21.
//

#include "BitBoard.h"
#include "OthelloGameBoard.h"

BitBoard::BitBoard(OthelloColor color) {
    this->color = color;

    long long bits = 0;
    switch(color) {
        case OthelloColor::Black: {
            BitBoard blackBoard = initBlack();
            bits = blackBoard.bits;
            break;
        }
        case OthelloColor::White: {
            BitBoard whiteBoard = initWhite();
            bits = whiteBoard.bits;
            break;
        }
    }

    this->bits = bits;
}

BitBoard::BitBoard(OthelloColor color, long long int bits) {
    this->color = color;
    this->bits = bits;
}

BitBoard::BitBoard(const BitBoard &oldBoard) {
    this->color = oldBoard.color;
    this->bits = oldBoard.bits;
}

BitBoard::BitBoard(const BitBoard &oldBoard, long long int bits) {
    this->color = oldBoard.color;
    this->bits = bits;
}

OthelloColor BitBoard::getColor() {
    return this->color;
}

std::array<std::array<int, 8>, 8> BitBoard::arrayRepresentation() {
    return OthelloGameBoard::binaryToArray(this->getBits());
}

// For below two methods, refer to: https://www.eothello.com/
BitBoard BitBoard::initBlack() {
    std::array<std::array<int, 8>, 8> blackBoard {};
    blackBoard.fill(std::array<int, 8> {0, 0, 0, 0, 0, 0, 0, 0 });

    // Starting from the very top left corner...
    blackBoard[3][4] = 1; // Over 3, down 4
    blackBoard[4][3] = 1; // Over 4, down 3

    long long conversionBits = OthelloGameBoard::arrayToBinary(blackBoard);

    return BitBoard(OthelloColor::Black, conversionBits);
}

BitBoard BitBoard::initWhite() {
    std::array<std::array<int, 8>, 8> whiteBoard {};
    whiteBoard.fill(std::array<int, 8> {0, 0, 0, 0, 0, 0, 0, 0 });

    // Starting from the very top left corner...
    whiteBoard[3][3] = 2; // Over 3, down 3
    whiteBoard[4][4] = 2; // Over 4, down 4

    long long conversionBits = OthelloGameBoard::arrayToBinary(whiteBoard);

    return BitBoard(OthelloColor::White, conversionBits);
}

long long BitBoard::getBits() {
    return this->bits;
}

std::array<int, 8> BitBoard::shifts() {
    const int columns = 8;
    return std::array<int, 8>({
        columns,

    });
}

BitBoard BitBoard::setCellState(int row, int col) {
    long long newBits = 1LL << (row * 8 + col);
    return BitBoard(*this, this->getBits() | newBits);
}

bool BitBoard::getCellState(int row, int col) {
    long long mask = 1LL << (row * 8 + col);
    return ((this->getBits() & mask) != 0);
}

int BitBoard::getCellCount() {
    int count = 0;
    long long bits = this->getBits();

    while(bits != 0) {
        bits &= bits - 1;
        count++;
    }

    return count;
}
