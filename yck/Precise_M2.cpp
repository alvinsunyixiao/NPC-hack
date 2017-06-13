#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <map>
#include <cmath>
using namespace std;
const int nmax = 17, smax = 1 << 16;
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

int f[smax][nmax], route[nmax], end_s, end;
double d[smax][nmax], mn_d = inf;
void DP()
{
	int s, t, i, j, k;
	for (s = 0; s < 1 << n; s++)
		for (i = 0; i <= n; i++)
			d[s][i] = inf;
	
	for (i = 1; i <= n; i++)
		f[1 << i - 1][i] = 0,
		d[1 << i - 1][i] = dis(T[0], T[i]);
	for (s = 0; s < 1 << n; s++)
	{
		k = __builtin_popcount(s);
		if (k <= sn)
			for (i = 1; i <= n; i++)
				if (T[i].c == step[k] && (s & (1 << i - 1)))
				{
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
				if (T[i].c == step[sn] && (s & (1 << i - 1)) && mn_d > d[s][i])
					end_s = s, 
					end = i,
					mn_d = d[s][i];
}

void output()
{
	string str[8] = {"", "Cedar", "PlaneTree", "Palm", "Pine", "MaidenhairTree", "Birch", "Polar"};
	char name[20], file[50];
	
	sprintf(file, "2-solution-%d.csv", int(mn_d + 0.5));
	FILE *out = fopen(file, "w");
	int s = end_s, t = end;
	for (int i = sn; i; t = f[s][t], s ^= (1 << route[i--] - 1))
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

