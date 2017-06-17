#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
const int       nmax = 2048;                    //maximum number of trees
const int       ccnt = 8;                       //number of tree types
const double    inf = 1e40;

struct  Position                                //tree representation
{
    int x;
    int y;
    int c;                                      //type ID

    Position(int x = 0, int y = 0, int c = 0)
    {
        this->x = x;
        this->y = y;
        this->c = c;
    }
}       T[nmax];

int     n;                                      //number of trees
int     step[nmax], sn;                         //type sequence, type length
int     bcnt[ccnt], bucket[ccnt][nmax];         //tree categorization
int     f[nmax][nmax];                          //trace-back ID
int     route[nmax];
int     end_idx;
double  d[nmax][nmax];                          //optimal distance
double  mn_d = inf;
double  dist[nmax][nmax];                       //distance between two trees

double dis(Position &a, Position &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void init(string root = "./")
{
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

    FILE *fm = fopen((root + "/map.csv").c_str(), "r");
    while (fscanf(fm, "%d,%d,%s", &x, &y, name) != EOF)
        T[++n] = Position(x, y, m[name]);

    fstream fg((root + "/guidebook.csv").c_str());
    while (fg.getline(name, 100, ','))
    {
        int l = strlen(name), i;
        for (i = 0; i < l; i++)
            if (name[i] < 'A' || name[i] > 'Z' && name[i] < 'a' || name[i] > 'z')
                break;
        name[i] = 0;
        step[++sn] = m[name];
    }

    //preprocess all possible distances
    for (int i = 0; i <= n; i++)
        for (int j = 0; j <= n; j++)
            dist[i][j] = dis(T[i], T[j]);

    //categorize trees
    for (int i = 1; i <= n; i++)
        bucket[T[i].c][++bcnt[T[i].c]] = i;
}

void DP()
{
    int i, j, k, x, y;

    //initialize all optimal distances to infinity
    for (i = 0; i <= sn; i++)
        for (j = 0; j <= n; j++)
            d[i][j] = inf;

    //calculate the distances of the first step
    for (j = 1; j <= n; j++)
        d[1][j] = dist[0][j],
        f[1][j] = 0;
    //calculate the distances of the following steps
    for (i = 2; i <= sn; i++)
        for (x = 1; x <= bcnt[step[i]]; x++)                //iterate over all trees of the current type
        {
            j = bucket[step[i]][x];
            for (y = 1; y <= bcnt[step[i - 1]]; y++)        //iterate over all trees of the previous type
            {
                k = bucket[step[i - 1]][y];
                if (d[i][j] > d[i - 1][k] + dist[j][k])
                    d[i][j] = d[i - 1][k] + dist[j][k],
                    f[i][j] = k;
            }
        }

    //find the optimal solution in the final step
    for (j = 1; j <= n; j++)
        if (step[sn] == T[j].c && mn_d > d[sn][j])
            mn_d = d[sn][j],
            end_idx = j;
}

void output()
{
    string str[8] = {"", "Cedar", "PlaneTree", "Palm", "Pine", "MaidenhairTree", "Birch", "Polar"};
    char name[20], file[50];

    sprintf(file, "solution-%d.csv", int(mn_d + 0.5));
    FILE *out = fopen(file, "w");
    int t = end_idx;
    for (int i = sn; i; t = f[i--][t])
        route[i] = t;
    for (int i = 1; i <= sn; i++)
    {
        str[T[route[i]].c].copy(name, str[T[route[i]].c].length());
        name[str[T[route[i]].c].length()] = 0;
        fprintf(out, "%d,%d,%s\n", T[route[i]].x, T[route[i]].y, name);
    }
}

int main(int argc, char *argv[])
{
	int TIME = clock();
    if (argc == 1)
        init();
    else
        init(string(argv[1]));
    DP();
    output();
    printf("%d\n", clock() - TIME);
    return 0;
}
