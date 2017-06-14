#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <ctime>

#include "position.hpp"
#include "global.hpp"

#define random(x) (rand()%x)

using namespace std;


double dis(Position &a, Position &b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "There should be exactly one parameter!" << endl;
        return 0;
    }

    srand((int)time(0));

    layout["Cedar"] = 0;
    layout["PlaneTree"] = 1;
    layout["Palm"] = 2;
    layout["Pine"] = 3;
    layout["MaidenhairTree"] = 4;
    layout["Birch"] = 5;
    layout["Poplar"] = 6;

    vector<Position> buf[7];

    ifstream map_fin(string(argv[1]) + "/map.csv");
    ifstream guide_fin(string(argv[1]) + "/guidebook.csv");

    string tmp;
    while (getline(map_fin, tmp)) {
        tmp.push_back(',');
        stringstream ss(tmp);
        int x, y;
        string str_x, str_y, l;
        getline(ss, str_x, ',');
        getline(ss, str_y, ',');
        x = stoi(str_x);
        y = stoi(str_y);
        getline(ss, l, ',');
        l.pop_back();
        cout << x << ' ' << y << ' ' << l << endl;
        buf[layout[l]].push_back({x, y, l});
    }

    while (getline(guide_fin, tmp, ',')) {
        if (tmp.find('\n') != string::npos) {
            tmp.pop_back();
            tmp.pop_back();
        }
    }

    return 0;
}
