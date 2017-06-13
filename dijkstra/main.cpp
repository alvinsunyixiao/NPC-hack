#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

#include "vertex.hpp"
#include "edge.hpp"
#include "vertexQueue.hpp"


Position* start_point() {
    Position *p = new Position();
    p->x = 0;
    p->y = 0;
    return p;
}

map<string, int> layout;

void dijkstra(VertexQueue &vqueue) {
    while (vqueue.size() > 0) {
        Vertex *current = vqueue.pop_front();
        current->marked = true;
        for (size_t i=0;i<current->target.size();i++) {
            if (current->target[i]->to->marked)
                continue;
            if (current->target[i]->to->queue_idx == -1)
                vqueue.insert(current->target[i]);
            else
                vqueue.decrement(current->target[i]->to->queue_idx, current->target[i]);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "There should be exactly two parameter!" << endl;
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
        buf[layout[l]].push_back({x, y});
    }

    vector<vector<Vertex*>> graph;
    vector<Vertex*> start;
    start.push_back(new Vertex(start_point(), "Start"));
    graph.push_back(start);

    while (getline(guide_fin, tmp, ',')) {
        if (tmp.find('\n') != string::npos) {
            tmp.pop_back();
            tmp.pop_back();
        }
        vector<Vertex*> row;
        for (size_t i=0; i<buf[layout[tmp]].size(); i++) {
            row.push_back(new Vertex(&(buf[layout[tmp]][i]), tmp));
            for (size_t j=0; j<graph.back().size(); j++) {
                graph.back()[j]->target.push_back(new Edge(graph.back()[j], row.back()));
            }
        }
        graph.push_back(row);
    }

    VertexQueue vqueue;
    start[0]->dis = 0;
    start[0]->queue_idx = 0;
    vqueue.data.push_back(start[0]);

    dijkstra(vqueue);

    Vertex *end;
    double min_dis = 10000000000;
    for (size_t i=0; i<graph.back().size(); i++) {
        if (graph.back()[i]->dis < min_dis) {
            end = graph.back()[i];
            min_dis = graph.back()[i]->dis;
        }
    }
    while(end) {
        end->display();
        end = end->parent;
    }

    return 0;
}
