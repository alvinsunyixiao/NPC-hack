#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>

using namespace std;

struct Position {
    int x;
    int y;
    int trace_id;
    double min_dis;
    string label;
};

Position* start_point() {
    Position *p = new Position();
    p->x = 0;
    p->y = 0;
    return p;
}

double dis(Position &a, Position &b) {
    return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

map<string, int> layout;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "There should be exactly one parameter!" << endl;
        return 0;
    }

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
        buf[layout[l]].push_back({x, y, -1, 0, l});
    }

    vector<vector<Position>> graph;
    vector<Position> start;
    start.push_back({0,0,-1,0,"Start"});
    graph.push_back(start);

    while (getline(guide_fin, tmp, ',')) {
        if (tmp.find('\n') != string::npos) {
            tmp.pop_back();
            tmp.pop_back();
        }
        vector<Position> row;
        for (size_t i=0; i<buf[layout[tmp]].size(); i++) {
            Position x = buf[layout[tmp]][i];
            x.min_dis = graph.back()[0].min_dis + dis(x, graph.back()[0]);
            for (size_t j=0; j<graph.back().size(); j++) {
                if (graph.back()[j].min_dis + dis(x, graph.back()[j]) <= x.min_dis) {
                    x.min_dis = graph.back()[j].min_dis + dis(x, graph.back()[j]);
                    x.trace_id = j;
                }
            }
            row.push_back(x);
        }
        graph.push_back(row);
    }

    Position end = graph.back()[0];
    double min_dis = end.min_dis;
    for (size_t i=0; i<graph.back().size(); i++) {
        if (min_dis > graph.back()[i].min_dis) {
            end = graph.back()[i];
            min_dis = graph.back()[i].min_dis;
        }
    }

    size_t level = graph.size()-1;
    while (end.trace_id != -1) {
        level--;
        printf("(%d, %d, %s) ", end.x, end.y, end.label.c_str());
        end = graph[level][end.trace_id];
    }

    return 0;
}
