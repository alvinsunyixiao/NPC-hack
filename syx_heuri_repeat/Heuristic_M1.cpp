/*****************************************************************
 *                                                               *
 * @file:       Heuristic_M1.cpp                                 *
 * @authors:    Chenkai Yu / Yixiao Sun / Yibing Du / Hongyu Li  *
 * @date:       06/17/2017                                       *
 *                                                               *
 *****************************************************************/
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

const int       nmax = 2048;
const double    inf = 1e40;

int             n;
int             grid_size;
int             step[nmax], sn;
int             bcnt[8], bucket[8][nmax];
double          dist[nmax][nmax];
vector<int>     grid[8][32][32];

int             route[nmax];
bool            vis[nmax];


struct  Position {
    int x, y, c;

    Position(int x = 0, int y = 0, int c = 0) {
        this->x = x;
        this->y = y;
        this->c = c;
    }
}       T[nmax];

bool    prob(double p) {
    unsigned int x = ((rand() << 15) + rand()) % (1 << 30);

    if (x < p * (1 << 30))
        return 1;
    else
        return 0;
}

//Find a nearby tree within the surround grids (Uniform Possibility)
void    neighbor_of(int t, int &tar) {
    int tmp = 0;
    int c = T[t].c;
    int x = T[t].x / grid_size;
    int y = T[t].y / grid_size;

    for (int i = x == 0 ? 0 : x - 1; i <= x + 1; i++)
        for (int j = y == 0 ? 0 : y - 1; j <= y + 1; j++)
            for (int k = 0; k < grid[c][i][j].size(); k++)
                if (rand() % ++tmp == 0)
                    tar = grid[c][i][j][k];
}

double  dis(Position &a, Position &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void    greedy() {
    int     i, j, k;
    int     min_idx;
    double  min_dis;

    for (i = 1; i <= sn; i++) {
        min_dis = inf;
        for (j = 1; j <= bcnt[step[i]]; j++) {
            k = bucket[step[i]][j];
            if (min_dis > dis(T[route[i - 1]], T[k]))
                min_dis = dis(T[route[i - 1]], T[k]),
                min_idx = k;
        }
        route[i] = min_idx;
        vis[min_idx] = 1;
    }
}

/*
 * @Struct Name:    Particle
 * @Purpose:        Data structure constructed to represent a possible solution while implementing the Simulated Annealing Algorithm
 */
struct  Particle {
    int     sq[nmax];
    int     cnt[nmax];
    int     last_id;
    double  e_change;

    Particle() {
        greedy();
        for (int i = 0; i <= n; i++)
            cnt[i] = 0;
        for (int i = 1; i <= sn; i++)
            cnt[sq[i] = route[i]]++;
        last_id = -1;
        e_change = 0;
    }

    Particle(Particle *a, Particle *b) {
        int cut1 = rand() % (sn + 1);

        for (int i = 0; i <= n; i++)
            cnt[i] = 0;
        for (int i = 0; i < cut1; i++)
            cnt[sq[i] = a->sq[i]]++;
        for (int i = cut1; i <= sn; i++)
            cnt[sq[i] = b->sq[i]]++;
        last_id = 1;
        e_change = 0;
    }

    //Change a single tree from the current routine with probablity calculated from energy change and constantly decreasing temperature
    void change(double tem) {
        int id = rand() % sn + 1;
        if (last_id > 1 && last_id < sn) {
            if (rand() & 1) id = last_id + 1;
            else id = last_id - 1;
        }

        int tar = sq[id];
        neighbor_of(sq[id], tar);

        double delta_e = 0;
        if (id != 1)  delta_e += dist[tar][sq[id - 1]] - dist[sq[id]][sq[id - 1]];
        if (id != sn) delta_e += dist[tar][sq[id + 1]] - dist[sq[id]][sq[id + 1]];

        if (delta_e <= 0 || prob(exp(-delta_e / tem))) {
            cnt[tar]++;
            cnt[sq[id]]--;
            sq[id] = tar;
            last_id = id;
            e_change += delta_e;
        }
        else last_id = -1;
    }

    //Display the solution in standard output (for debug only)
    void output() {
        for (int i = 1; i <= sn; i++)
            printf("%d ", sq[i]);
        printf("\n");
    }

    //Check for re-visited trees
    bool validity() {
        for (int i = 0; i <= n; i++)
            if (cnt[i] > 1)
            return false;
        return true;
    }

    //Calculate routine length for current routine configuration
    double length() {
        double res = 0;
        for (int i = 1; i <= sn; i++)
            res += dist[sq[i - 1]][sq[i]];
        return res;
    }
};

bool path_cmp(Particle *a, Particle *b) {
    return (a->length() < b->length());
}

struct Population {
    size_t              SIZE;
    int                 epoch;
    double              min_dis;
    double              max_dis;
    double              mutation_rate;
    double              temperature;
    vector<Particle*>   pool;

    Population(size_t s, double mutation_rate = 0.3) {
        this->SIZE = s;
        this->mutation_rate = mutation_rate;

        min_dis = inf;
        max_dis = 0;
        epoch = 0;
        temperature = 50;
        for (size_t i=0; i<SIZE; i++) {
            pool.push_back(new Particle());
        }
    }

    void    survive() {
        double survival_rate = 1;
        double tmp_l;

        min_dis = inf;
        max_dis = 0;
        for (size_t i = 0; i < pool.size(); i++) {
            tmp_l = pool[i]->length();
            min_dis = tmp_l < min_dis ? tmp_l : min_dis;
            max_dis = tmp_l > max_dis ? tmp_l : max_dis;
        }
        for (size_t i = 0; i < pool.size(); i++) {
            if (max_dis != min_dis)
                survival_rate = (max_dis-pool[i]->length())/(max_dis-min_dis);
            if (!prob(survival_rate) && size() > 2) {
                delete pool[i];
                swap(pool[i], pool.back());
                pool.pop_back();
                i--;
            }
        }
    }

    void    fill() {
        size_t old_size = size();

        while (pool.size() < SIZE) {
            int idx1 = rand() % old_size;
            int idx2 = rand() % old_size;
            while (idx1 == idx2)
                idx2 = rand() % old_size;
            pool.push_back(new Particle(pool[idx1], pool[idx2]));
        }
    }

    void    mutate() {
        sort(pool.begin(), pool.end(), path_cmp);
        for (size_t i=pool.size() * 0.3; i<pool.size(); i++)
            pool[i]->change(temperature);
        temperature *= 0.9996;
    }

    size_t  size() {
        return pool.size();
    }

    void    evolve() {
        survive();
        printf("Epoch: %d Min Distance: %.3f Survived: %.2f Size: %zu\n", epoch, min_dis, pool.size()*1.0/ SIZE, SIZE);
        fill();
        mutate();
        epoch++;
    }
};

void    init(string root = ".") {
    map <string, int> m;
    m["Cedar"]          = 1;
    m["PlaneTree"]      = 2;
    m["Palm"]           = 3;
    m["Pine"]           = 4;
    m["MaidenhairTree"] = 5;
    m["Birch"]          = 6;
    m["Polar"]          = 7;

    int x, y;
    char name[20];

    srand(time(0));

    FILE *fm = fopen((root+"/map.csv").c_str(), "r");
    while (fscanf(fm, "%d,%d,%s", &x, &y, name) != EOF)
        T[++n] = Position(x, y, m[name]);

    fstream fg((root+"/guidebook.csv").c_str());
    while (fg.getline(name, 20, ',')) {
        int l = strlen(name), i;
        for (i = 0; i < l; i++)
            if ((name[i] < 'A' || name[i] > 'Z') && (name[i] < 'a' || name[i] > 'z'))
                break;
        name[i] = 0;
        step[++sn] = m[name];
    }

    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            dist[i][j] = dis(T[i], T[j]);

    for (int i = 1; i <= n; i++)
        bucket[T[i].c][++bcnt[T[i].c]] = i;

    //Divide global space into grids and initialize a grid representation of tree points
    grid_size = max(sqrt(n) * 4, 50.0);
    for (int i = 1; i <= n; i++)
        grid[T[i].c][T[i].x / grid_size][T[i].y / grid_size].push_back(i);
}

Particle p;

// Simulated Annealing Algorithm
void    SA() {
    p = Particle();

    double ts = 30, te = 1, last_pc = 0, pc, tot = log(ts / te);
    for (double tem = ts; tem > te; tem *= 0.99999999) {
        p.change(tem);
        pc = log(ts / tem) / tot;
        if (pc - last_pc > 0.01) {
            printf("%d%%. E change is %lf. len = %lf. %svalid\n", int(pc * 100), p.e_change, p.length(), p.validity() ? "" : "in");
            last_pc = pc;
        }
    }
    printf(" %svalid\n", p.validity() ? "" : "in");
}

void    output(double length) {
    string str[8] = {"", "Cedar", "PlaneTree", "Palm", "Pine", "MaidenhairTree", "Birch", "Polar"};
    char name[20], file[50];

    sprintf(file, "solution-%d.csv", int(length + 0.5));
    FILE *out = fopen(file, "w");
    for (int i = 1; i <= sn; i++) {
        str[T[route[i]].c].copy(name, str[T[route[i]].c].length());
        name[str[T[route[i]].c].length()] = 0;
        fprintf(out, "%d,%d,%s\n", T[route[i]].x, T[route[i]].y, name);
    }
}

int     main(int argc, char *argv[]) {
    if (argc == 2)
        init(argv[1]);
    else
        init();

    double min_dis = inf;

    Population pop(200, 0.3);
    for (int i = 0; i < 9999; i++)
        pop.evolve();
    //output(min_dis);
    return 0;
}

