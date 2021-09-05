//
// Created by Harry Burnett on 8/29/21.
//

#include <bitset>
#include <string>

#include "OthelloGameBoard.h"
#include "../Logger.h"

OthelloGameBoard::OthelloGameBoard(BitBoard black, BitBoard white) : m_black(black), m_white(white) {
    this->is_finished = false;
    this->m_board = std::array<std::array<int, 8>, 8> {};

    // Initializes an empty gameboard (8 x 8) to zeroes.
    this->m_board.fill(std::array<int, 8> { 0, 0, 0, 0, 0, 0, 0, 0 });

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            int blackPos = getBlack().arrayRepresentation()[i][j];
            int whitePos = getWhite().arrayRepresentation()[i][j];

            if(blackPos != 0) {
                this->m_board[i][j] = blackPos;
            } else if(whitePos != 0) {
                this->m_board[i][j] = whitePos;
            }
        }
    }
}

long long OthelloGameBoard::arrayToBinary(std::array<std::array<int, 8>, 8> pieces) {
    long long x = 0; // x could be either black or white.

    for(int i = 0; i < 64; i++) {
        std::string binary = "0000000000000000000000000000000000000000000000000000000000000000";
        binary = binary.substr(i + 1) + "1" + binary.substr(0, i);

        switch(pieces[i / 8][i % 8])
        {
            case 0:
                break;
            default:
                x += OthelloGameBoard::stringToBinary(binary);
                break;
        }
    }

    return x;
}

OthelloGameBoard OthelloGameBoard::createBoard(long long b, long long w) {
    BitBoard blackBitBoard = BitBoard(OthelloColor::Black, b);
    BitBoard whiteBitBoard = BitBoard(OthelloColor::White, w);

    return OthelloGameBoard(blackBitBoard, whiteBitBoard);
}

std::array<std::array<int, 8>, 8> OthelloGameBoard::binaryToArray(long long bitboardBits) {
    std::array<std::array<int, 8>, 8> board {};

    board.fill(std::array<int, 8> { 0, 0, 0, 0, 0, 0, 0, 0 });

    for(int i = 0; i < 64; i++) {
        if(((bitboardBits >> i) & 1) == 1) {
            board[i / 8][i % 8] = 1;
        }
    }

    return board;
}

void OthelloGameBoard::drawBoard() {
    Logger::logComment("\t ABCDEFGH");
    for(int i = 0; i < 8; i++) {
        // We need to use manual prints in this specific case.
        std::cout << "C " << i + 1 << ". ";

        for(int j = 0; j < 8; j++) {
            int blackPiece = this->getBlack().arrayRepresentation()[i][j];
            int whitePiece = this->getWhite().arrayRepresentation()[i][j];

            // For the sake of a nice display, black pieces are represented as
            // 1s and white pieces are represented as 2s. *Rather* than reading
            // directly from the array.

            // Hopefully this doesn't come back to haunt us later...

            if(blackPiece != 0) {
                Logger::logComment("1", false);
            } else if(whitePiece != 0) {
                Logger::logComment("2", false);
            } else {
                Logger::logComment("0", false);
            }
        }

        std::cout << std::endl;
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
long long OthelloGameBoard::stringToBinary(std::string binary) {
    try {
        if(binary[0] == '0') {
            return std::bitset<64>(binary).to_ulong();
        }

        // Converts input into appropriate long long from 2's compliment.
        return std::bitset<64>("1" + binary.substr(2)).to_ulong() * 2;
    } catch(std::exception &e) { // Would normally limit to a specific exception, but we log the same message either way.
        Logger::logComment("Failed to convert string '" + binary + "' to long.\nException: " + e.what());
    }

    return -1;
}
#pragma clang diagnostic pop

BitBoard OthelloGameBoard::getBlack() {
    return this->m_black;
}

BitBoard OthelloGameBoard::getWhite() {
    return this->m_white;
}

std::array<std::array<int, 8>, 8> OthelloGameBoard::getBoard() {
    return this->m_board;
}
