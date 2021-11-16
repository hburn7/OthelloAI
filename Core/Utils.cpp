//
// Created by hburn7 on 9/20/21.
//

#include "Utils.h"

int Utils::getColFromChar(char col) {
    switch(col) {
        case 'a': return 0;
        case 'b': return 1;
        case 'c': return 2;
        case 'd': return 3;
        case 'e': return 4;
        case 'f': return 5;
        case 'g': return 6;
        case 'h': return 7;
        default:  return -1;
    }
}

char Utils::getColFromInt(int column) {
    const std::string columns = "abcdefgh";
    return columns.at(column);
}

std::tuple<int, char> Utils::posToRowCol(int pos) {
    int row = 7 - (pos / 8);
    int col = 7 - (pos % 8);

    if(row < 0) {
        row = -row;
    }
    if(col < 0) {
        col = -col;
    }

    char c = getColFromInt(col);

    return std::tuple<int, char>(row + 1, c);
}
