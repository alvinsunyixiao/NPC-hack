#ifndef POPULATION_H
# define POPULATION_H

#include <vector>
#include <string>
#include "position.hpp"
#include "global.hpp"

using namespace std;

class Path {
public:
    int         dis;
    vector<int> arr;

    Path(vector<Position> *map,
         vector<string> &guide);
    void    eval(vector<Position> *map, vector<string> &guide);
    double  calc_dis(Position &a, Position &b);

};

class Population {
public:
    int             SIZE;
    int             epoch;
    vector<Path*>   pool;

    Population(int s,
               vector<Position> *buf,
               vector<string> &guide);
    double mutation_rate();
};

#endif
