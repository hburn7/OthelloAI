//
// Created by hburn7 on 10/21/21.
//

#ifndef OTHELLOPROJECT_CPP_CONFIG_H
#define OTHELLOPROJECT_CPP_CONFIG_H


class Config {
public:
    Config(bool interactive, int game_time);

    bool isInteractive() const;
    double getMoveTime() const;
private:
    bool m_interactive;
    double m_move_time;
};


#endif //OTHELLOPROJECT_CPP_CONFIG_H
