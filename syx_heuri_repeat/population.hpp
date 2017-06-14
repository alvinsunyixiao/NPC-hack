#ifndef POPULATION_H
# define POPULATION_H

#include <vector>
#include <string>
#include "position.hpp"

using namespace std;

class Path {
public:
    double      dis;
    vector<int> arr;

    Path();
    Path(Path *a, Path *b);

    void    eval();
    void    mutate(double rate);
    double  calc_dis(Position &a, Position &b);

};

class Population {
public:
    size_t          SIZE;
    int             epoch;
    double          min_dis;
    double          max_dis;
    double          mutation_rate;
    vector<Path*>   pool;

    Population(size_t s, double mutation_rate = 0.3);

    void            survive();
    void            fill();
    void            mutate();
    size_t          size();
    void            evolve();
};

#endif
