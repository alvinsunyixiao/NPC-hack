#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>

#include "global.hpp"
#include "population.hpp"

#define N 999999

using namespace std;

bool path_cmp(Path *a, Path *b) {
    return (a->dis < b->dis);
}

bool pass(double rate) {
    int     rand_int = rand()%(N+1);
    double  p = 1.0 * rand_int / N;
    if (p <= rate)
        return true;
    else
        return false;
}

Path::Path() {
    int     min_idx = 0;
    int     prev_idx = 0;
    double  min_dis = inf;

    arr.push_back(0);
    for (size_t i=1; i<guide.size(); i++) {
        for (size_t j = 0; j < nodes[layout[guide[i]]].size(); j++)
            if (min_dis > calc_dis(nodes[layout[guide[i]]][j], nodes[layout[guide[i-1]]][prev_idx])) {
                min_dis = calc_dis(nodes[layout[guide[i]]][j], nodes[layout[guide[i-1]]][prev_idx]);
                min_idx = j;
            }
        prev_idx = min_idx;
        arr.push_back(min_idx);
        min_dis = inf;
        //arr.push_back((int)(rand()%nodes[layout[guide[i]]].size()));
    }
    eval();
}

Path::Path(Path *a, Path *b) {
    size_t cut = rand() % a->arr.size();
    for (size_t i=0; i<cut; i++) {
        arr.push_back(a->arr[i]);
    }
    for (size_t i=cut; i<a->arr.size(); i++) {
        arr.push_back(b->arr[i]);
    }
    eval();
}

void Path::eval() {
    dis = 0;

    for (size_t i=1; i<guide.size(); i++) {
        dis += calc_dis(nodes[layout[guide[i]]][arr[i]],
                        nodes[layout[guide[i-1]]][arr[i-1]]);
    }
}

void Path::mutate(double rate) {
    size_t idx = rand() % arr.size();
    if (pass(rate))
        arr[idx] = (int)(rand()%nodes[layout[guide[idx]]].size());
    eval();
}

double Path::calc_dis(Position &a, Position &b) {
    return sqrt(pow(1.0*a.x-b.x, 2) + pow(1.0*a.y-b.y, 2));
}


Population::Population(size_t size, double mutation_rate) {
    this->SIZE = size;
    this->mutation_rate = mutation_rate;

    min_dis = inf;
    max_dis = 0;
    epoch = 0;
    for (size_t i=0; i<SIZE; i++) {
        pool.push_back(new Path());
    }
}

size_t Population::size() {
    return pool.size();
}

void Population::survive() {
    /*double survival_rate;

    min_dis = inf;
    max_dis = 0;

    for (size_t i=0; i<pool.size(); i++) {
        if (pool[i]->dis < min_dis)
            min_dis = pool[i]->dis;
        if (pool[i]->dis > max_dis)
            max_dis = pool[i]->dis;
    }
    if (min_dis == max_dis) {
        mutate();
        return;
    }
    for (size_t i=0; i<pool.size(); i++) {
        survival_rate = (max_dis-pool[i]->dis)/(max_dis-min_dis);
        survival_rate = pow(survival_rate, 2);
        if (!pass(survival_rate) && size() >= 2) {
            delete pool[i];
            swap(pool[i], pool.back());
            pool.pop_back();
            i--;
        }
    }
*/
    sort(pool.begin(), pool.end(), path_cmp);
    min_dis = pool[0]->dis;
    while (size() > SIZE * 0.5) {
        delete pool.back();
        pool.pop_back();
    }
}

void Population::fill() {
    while (pool.size() < SIZE * 0.6) {
        pool.push_back(new Path());
        if (pool.back()->dis >= min_dis)
            pool.back()->mutate(mutation_rate);
    }
    size_t old_size = size();
    while (pool.size() < SIZE) {
        int idx1 = rand() % old_size;
        int idx2 = rand() % old_size;
        while (idx1 == idx2) {
            idx2 = rand() % old_size;
        }
        pool.push_back(new Path(pool[idx1], pool[idx2]));
        if (pool.back()->dis >= min_dis)
            pool.back()->mutate(mutation_rate);
    }
}

void Population::mutate() {
    for (size_t i=1; i<pool.size(); i++) {
        pool[i]->mutate(mutation_rate);
    }
}

void Population::evolve() {
    survive();
    printf("Epoch: %d Min Distance: %.3f Survived: %.2f Size: %zu\n", epoch, min_dis, pool.size()*1.0/ SIZE, SIZE);
    //mutation_rate = pow(pool.size()*1.0/SIZE, 10);
    fill();
    //mutate();
    epoch++;
}
