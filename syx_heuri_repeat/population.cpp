#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <cstdio>

#include "global.hpp"
#include "population.hpp"

#define N 9999

using namespace std;

Path::Path(vector<Position> *map,
           vector<string> &guide) {
    for (size_t i=0; i<guide.size(); i++) {
        arr.push_back((int)(rand()%map[layout[guide[i]]].size()));
    }
    eval(map, guide);
}

void Path::eval(vector<Position> *map,
                vector<string> &guide) {
    dis = 0;
    for (size_t i=1; i<guide.size(); i++) {
        dis += calc_dis(map[layout[guide[i]]][arr[i]],
                        map[layout[guide[i-1]]][arr[i-1]]);
    }
}

Population::Population(int s,
                       vector<Position> *map,
                       vector<string> &guide) {
    SIZE = s;
    epoch = 0;
    for (int i=0; i<SIZE; i++) {
        pool.push_back(new Path(map, guide));
    }
}

double Population::mutation_rate() {
    return 1.0 / (3+epoch);
}
