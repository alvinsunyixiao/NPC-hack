#ifndef POSITION_H
# define POSITION_H

#include <string>

using namespace std;

struct Position {
    int         x;
    int         y;
    std::string label;
    Position(int x = 0, int y = 0, string label = "Start") {
        this->x = x;
        this->y = y;
        this->label = label;
    }
};

#endif
