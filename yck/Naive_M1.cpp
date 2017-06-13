#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <cmath>
using namespace std;
const int nmax = 2048;
const double inf = 1e40;

struct Position
{
	int x, y, c;
	Position(int x = 0, int y = 0, int c = 0)
	{
		this->x = x;
		this->y = y;
		this->c = c;
	}
} T[nmax];

int n, step[nmax], sn;

void init()
{
	map <string, int> m;
	m["Cedar"] = 1;
	m["PlaneTree"] = 2;
	m["Palm"] = 3;
	m["Pine"] = 4;
	m["MaidenhairTree"] = 5;
	m["Birch"] = 6;
	m["Polar"] = 7;
	
	int x, y;
	char name[20];
	
	FILE *fm = fopen("map.csv", "r");
	while (fscanf(fm, "%d,%d,%s", &x, &y, name) != EOF)
		T[++n] = Position(x, y, m[name]);
	
	fstream fg("guidebook.csv");
	while (fg.getline(name, 100, ','))
	{
		int l = strlen(name), i;
		for (i = 0; i < l; i++)
			if (name[i] < 'A' || name[i] > 'Z' && name[i] < 'a' || name[i] > 'z')
				break;
		name[i] = 0;
		step[++sn] = m[name];
	}
}

double dis(Position &a, Position &b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

int f[nmax][nmax], route[nmax], end;
double d[nmax][nmax], mn_d = inf;
void DP()
{
	int i, j, k;
	for (i = 0; i <= sn; i++)
		for (j = 0; j <= n; j++)
			d[i][j] = inf;
	
	for (j = 1; j <= n; j++)
		f[1][j] = 0,
		d[1][j] = dis(T[0], T[j]);
	for (i = 2; i <= sn; i++)
		for (j = 1; j <= n; j++)
			if (step[i] == T[j].c)
				for (k = 1; k <= n; k++)
					if (step[i - 1] == T[k].c && d[i][j] > d[i - 1][k] + dis(T[j], T[k]))
						f[i][j] = k,
						d[i][j] = d[i - 1][k] + dis(T[j], T[k]);
	
	for (j = 1; j <= n; j++)
		if (step[sn] == T[j].c && mn_d > d[sn][j])
			end = j,
			mn_d = d[sn][j];
}

void output()
{
	string str[8] = {"", "Cedar", "PlaneTree", "Palm", "Pine", "MaidenhairTree", "Birch", "Polar"};
	char name[20], file[50];
	
	sprintf(file, "solution-%d.csv", int(mn_d + 0.5));
	FILE *out = fopen(file, "w");
	int t = end;
	for (int i = sn; i; t = f[i--][t])
		route[i] = t;
	for (int i = 1; i <= sn; i++)
	{
		str[T[route[i]].c].copy(name, str[T[route[i]].c].length());
		name[str[T[route[i]].c].length()] = 0;
		fprintf(out, "%d,%d,%s\n", T[route[i]].x, T[route[i]].y, name);
	}
}

int main()
{
	init();
	DP();
	output();
	return 0;
}
