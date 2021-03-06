//
// Created by hburn7 on 9/20/21.
//

#ifndef OTHELLOPROJECT_CPP_UTILS_H
#define OTHELLOPROJECT_CPP_UTILS_H

#include <string>
#include <tuple>

class Utils {
public:
    /**
    * Othello board is in lettered columns, a-h. This will convert the character to the desired index.
    */
    static int getColFromChar(char c);
    /**
     * Converts an integer representation of a column into a char to be printed out.
     */
    static char getColFromInt(int column);
    /**
     *
     * @param pos The integer position on the bitboard, ranging from 0-63 inclusive.
     * @return A tuple containing the row as an integer and the column as a char.
     */
    static std::tuple<int, char> posToRowCol(int pos);
};


#endif //OTHELLOPROJECT_CPP_UTILS_H
