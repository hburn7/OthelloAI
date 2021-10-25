//
// Created by stage on 10/21/21.
//

#ifndef OTHELLOPROJECT_CPP_CONFIG_H
#define OTHELLOPROJECT_CPP_CONFIG_H


class Config {
public:
    Config(bool interactive, int game_time);

    bool isInteractive() const;
    void setInteractive(bool interactive);

    int getMoveTime() const;

    int getTimeRemaining() const;
    void setTimeRemaining(int mTimeRemaining);
private:
    bool m_interactive;
    int m_move_time;
    int m_time_remaining;
};


#endif //OTHELLOPROJECT_CPP_CONFIG_H
