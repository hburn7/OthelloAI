//
// Created by stage on 10/31/21.
//

#ifndef OTHELLOPROJECT_CPP_MOVE_H
#define OTHELLOPROJECT_CPP_MOVE_H


class Move {
public:
    Move(); // Pass by default
    Move(int pos, int value);

    int getPos() const;
    void setPos(int pos);

    int getValue() const;
    void setValue(int value);

    bool isPass();

    friend bool operator < (const Move &lhs, const Move &rhs);
private:
    int m_pos;
    int m_value;
    bool m_isPass;
};

#endif //OTHELLOPROJECT_CPP_MOVE_H
