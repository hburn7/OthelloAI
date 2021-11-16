//
// Created by stage on 10/31/21.
//

#ifndef OTHELLOPROJECT_CPP_MOVE_H
#define OTHELLOPROJECT_CPP_MOVE_H


class Move {
public:
    /**
     * Generates a default move (pass).
     */
    Move();
    /**
     * Creates a move with the given pos and value.
     * @param pos The position on the gameboard, with respect to H8 = 0 and A1 = 63 [0-63 inclusive]
     * @param value The value of the move. Populated by alphaBeta.
     */
    Move(int pos, int value);

    // Getters and setters

    int getPos() const;
    int getValue() const;

    void setValue(int value);

    /**
     * @return Whether the move is a pass move.
     */
    bool isPass();

    /**
     * Comparator to show whether one move is less valuable than another move.
     * Only used for priority queue generation of moves.
     */
    friend bool operator < (const Move &lhs, const Move &rhs);
private:
    int m_pos;
    int m_value;
    bool m_isPass;
};

#endif //OTHELLOPROJECT_CPP_MOVE_H
