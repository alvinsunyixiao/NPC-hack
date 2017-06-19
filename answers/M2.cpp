/*****************************************************************
 *                                                               *
 * @file:       M2.cpp                                           *
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

using namespace std;

const int nmax = 2048;
const double inf = 1e40;
int n = 0, sn = 0;
int step[nmax];

struct  Position {
    int x, y, c;
    Position(int x = 0, int y = 0, int c = 0) {
        this->x = x;
        this->y = y;
        this->c = c;
    }
}       T[nmax];

namespace Precise {
    const int nmax = 17, smax = 1 << 16;

    double dis(Position &a, Position &b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    int f[smax][nmax], route[nmax], end_s, end_idx;
    double d[smax][nmax], min_dis = inf;
    void DP() {
        int s, t, i, j, k;
        for (s = 0; s < 1 << n; s++)
            for (i = 0; i <= n; i++)
                d[s][i] = inf;

        for (i = 1; i <= n; i++)
            f[1 << i - 1][i] = 0,
            d[1 << i - 1][i] = dis(T[0], T[i]);
        for (s = 0; s < 1 << n; s++) {
            k = __builtin_popcount(s);
            if (k <= sn)
                for (i = 1; i <= n; i++)
                    if (T[i].c == step[k] && (s & (1 << i - 1))) {
                        t = s ^ (1 << i - 1);
                        for (j = 1; j <= n; j++)
                            if (T[j].c == step[k - 1] && (t & (1 << j - 1)) && d[s][i] > d[t][j] + dis(T[i], T[j]))
                                f[s][i] = j,
                                d[s][i] = d[t][j] + dis(T[i], T[j]);
                    }
        }

        for (s = 0; s < 1 << n; s++)
            if (__builtin_popcount(s) == sn)
                for (i = 1; i <= n; i++)
                    if (T[i].c == step[sn] && (s & (1 << i - 1)) && min_dis > d[s][i])
                        end_s = s,
                        end_idx = i,
                        min_dis = d[s][i];
    }

    void output() {
        string str[8] = {"", "Cedar", "PlaneTree", "Palm", "Pine", "MaidenhairTree", "Birch", "Polar"};
        char name[20], file[50];

        sprintf(file, "solution-%d.csv", int(min_dis + 0.5));
        FILE *out = fopen(file, "w");
        int s = end_s, t = end_idx;
        for (int i = sn; i; t = f[s][t], s ^= (1 << route[i--] - 1))
            route[i] = t;
        for (int i = 1; i <= sn; i++) {
            str[T[route[i]].c].copy(name, str[T[route[i]].c].length());
            name[str[T[route[i]].c].length()] = 0;
            fprintf(out, "%d,%d,%s\n", T[route[i]].x, T[route[i]].y, name);
        }
    }
}

namespace Heuristic {
    const int nmax = 2048;

    double min_dis;
    int route[nmax];
    bool vis[nmax];

    int bcnt[8], bucket[8][nmax];
    double dist[nmax][nmax];
    int grid_size;
    vector<int> grid[8][32][32];


    bool prob(double p) {
        //if (rand() == 0) printf("%lf ", p);
        unsigned int x = ((rand() << 15) + rand()) % (1 << 30);
        if (x < p * (1 << 30))
            return 1;
        else
            return 0;
    }

    void neighbor_of(int t, int &tar) {
        int c = T[t].c, x = T[t].x / grid_size, y = T[t].y / grid_size, tmp = 0;
        for (int i = x == 0 ? 0 : x - 1; i <= x + 1; i++)
            for (int j = y == 0 ? 0 : y - 1; j <= y + 1; j++)
                for (int k = 0; k < grid[c][i][j].size(); k++)
                    if (rand() % ++tmp == 0)
                        tar = grid[c][i][j][k];
    }

    double dis(Position &a, Position &b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    void work() {
        int i, j, k;
        int min_idx;

        for (i = 1; i <= sn; i++) {
            min_dis = inf;
            for (j = 1; j <= bcnt[step[i]]; j++) {
                k = bucket[step[i]][j];
                if (!vis[k] && min_dis > dis(T[route[i - 1]], T[k]))
                    min_dis = dis(T[route[i - 1]], T[k]),
                    min_idx = k;
            }
            route[i] = min_idx;
            vis[min_idx] = 1;
        }
    }

    struct Particle {
        int sq[nmax], cnt[nmax], last_id;
        double e_change;
        Particle() {
            work();
            for (int i = 0; i <= n; i++)
                cnt[i] = 0;
            for (int i = 1; i <= sn; i++)
                cnt[sq[i] = route[i]]++;
            last_id = -1;
            e_change = 0;
        }
        void change(double tem, double alpha = 0) {
            int id = rand() % sn + 1;
            if (last_id > 1 && last_id < sn) {
                if (rand() & 1) id = last_id + 1;
                else id = last_id - 1;
            }

            int tar = sq[id];//bucket[T[sq[id]].c][rand() % bcnt[T[sq[id]].c] + 1];
            neighbor_of(sq[id], tar);

            double delta_e = alpha * ((cnt[tar] >= 1) - (cnt[sq[id]] >= 2));
            if (id != 1)  delta_e += dist[tar][sq[id - 1]] - dist[sq[id]][sq[id - 1]];
            if (id != sn) delta_e += dist[tar][sq[id + 1]] - dist[sq[id]][sq[id + 1]];

            if (delta_e <= 0 || prob(exp(-delta_e / tem))) {
                cnt[tar]++;
                cnt[sq[id]]--;
                sq[id] = tar;
                last_id = id;
                e_change += delta_e;
                //if (tem < 12 && rand() < 10) printf(" ");
            }
            else last_id = -1;
        }
        void output() {
            for (int i = 1; i <= sn; i++)
                printf("%d ", sq[i]);
            printf("\n");
        }
        bool validity() {
            for (int i = 0; i <= n; i++)
                if (cnt[i] > 1)
                return false;
            return true;
        }
        double length() {
            double res = 0;
            for (int i = 1; i <= sn; i++)
                res += dist[sq[i - 1]][sq[i]];
            return res;
        }
    };

    void init() {
        for (int i = 0; i <= n; i++)
            for (int j = 0; j <= n; j++)
                dist[i][j] = dis(T[i], T[j]);

        for (int i = 1; i <= n; i++)
            bucket[T[i].c][++bcnt[T[i].c]] = i;

        grid_size = max(sqrt(n) * 4, 50.0);
        for (int i = 1; i <= n; i++)
            grid[T[i].c][T[i].x / grid_size][T[i].y / grid_size].push_back(i);
    }

    Particle p;
    void SA() {
        p = Particle();

        //printf(" %d\n", p.validity());

        double ts = 30, te = 1, last_pc = 0, pc, tot = log(ts / te), alpha = 30;
        for (double tem = ts; tem > te; tem *= 0.99999999) {
            p.change(tem, alpha);
            pc = log(ts / tem) / tot;
            if (pc - last_pc > 0.01) {
                printf("%d%%. E change is %lf. len = %lf. %svalid\n", int(pc * 100), p.e_change, p.length(), p.validity() ? "" : "in");
                last_pc = pc;
                alpha *= 2;

                //printf(" %d\n", p.validity());
            }
        }

        printf(" %svalid\n", p.validity() ? "" : "in");
        //p.output();
    }

    void output(double length) {
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
}

int main() {
    srand(time(0));

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

    FILE *fm = fopen("map.csv", "r");
    while (fscanf(fm, "%d,%d,%s", &x, &y, name) != EOF)
        T[++n] = Position(x, y, m[name]);

    fstream fg("guidebook.csv");
    while (fg.getline(name, 20, ',')) {
        int l = strlen(name), i;
        for (i = 0; i < l; i++)
            if (name[i] < 'A' || name[i] > 'Z' && name[i] < 'a' || name[i] > 'z')
                break;
        name[i] = 0;
        step[++sn] = m[name];
    }

    if (sn < Precise::nmax) {
        Precise::DP();
        Precise::output();
    }
    else {
        Heuristic::init();

        double min_dis = inf;
        Heuristic::Particle *opt;

        for (int t = 0; t < 3; t++) {
            printf("%d\n", t);
            Heuristic::SA();
            if (min_dis > Heuristic::p.length()) {
                min_dis = Heuristic::p.length();
                opt = &(Heuristic::p);
            }
        }

        for (int i = 1; i <= sn; i++)
            Heuristic::route[i] = opt->sq[i];

        Heuristic::output(min_dis);
    }
    return 0;
}
