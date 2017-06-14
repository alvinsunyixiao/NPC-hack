#ifndef GLOBAL_H
# define GLOBAL_H

#include <map>
#include <string>
#include <vector>
#include "position.hpp"

extern const double inf;

extern std::map<std::string, int>  layout;
extern std::vector<Position>       nodes[7];
extern std::vector<std::string>    guide;

#endif
