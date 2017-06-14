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
    for (size_t i=0; i<guide.size(); i++) {
        arr.push_back((int)(rand()%nodes[layout[guide[i]]].size()));
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
    Position &first = nodes[layout[guide[0]]][arr[0]];
    dis = sqrt(first.x * first.x + first.y * first.y);

    for (size_t i=1; i<guide.size(); i++) {
        dis += calc_dis(nodes[layout[guide[i]]][arr[i]],
                        nodes[layout[guide[i-1]]][arr[i-1]]);
    }
}

void Path::mutate(double rate) {
    for (size_t i=0; i<arr.size(); i++)
        if (pass(rate))
            arr[i] = (int)(rand()%nodes[layout[guide[i]]].size());
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
    double survival_rate;

    min_dis = inf;
    max_dis = 0;

    for (size_t i=0; i<pool.size(); i++) {
        if (pool[i]->dis < min_dis)
            min_dis = pool[i]->dis;
        if (pool[i]->dis > max_dis)
            max_dis = pool[i]->dis;
    }
    for (size_t i=0; i<pool.size(); i++) {
        if (max_dis == min_dis)
            survival_rate = 1;
        else
            survival_rate = (max_dis-pool[i]->dis)/(max_dis-min_dis);
        survival_rate = pow(survival_rate, 1);
        if (!pass(survival_rate)) {
            delete pool[i];
            swap(pool[i], pool.back());
            pool.pop_back();
            i--;
        }
    }
    /*
    sort(pool.begin(), pool.end(), path_cmp);
    min_dis = pool[0]->dis;
    while (size() > SIZE * 0.5) {
        delete pool.back();
        pool.pop_back();
    }*/
}

void Population::fill() {
    size_t old_size = size();
    while (pool.size() < SIZE) {
        int idx1 = rand() % old_size;
        int idx2 = rand() % old_size;
        while (idx1 == idx2) {
            idx2 = rand() % old_size;
        }
        pool.push_back(new Path(pool[idx1], pool[idx2]));
        //pool.back()->mutate(mutation_rate);
    }
}

void Population::mutate() {
    for (size_t i=0; i<pool.size(); i++) {
        if (pool[i]->dis != min_dis)
            pool[i]->mutate(mutation_rate);
    }
}

void Population::evolve() {
    survive();
    printf("Epoch: %d Min Distance: %.3f Survived: %.2f Size: %zu\n", epoch, min_dis, pool.size()*1.0/ SIZE, SIZE);
    //mutation_rate = pow(pool.size()*1.0/SIZE, 10);
    fill();
    mutate();
    epoch++;
}
