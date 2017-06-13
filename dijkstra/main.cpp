#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <fstream>
#include <sstream>

#include "vertex.hpp"
#include "edge.hpp"
#include "vertexQueue.hpp"


vector<Vertex*> data_process(ifstream &fin) {
    vector<Vertex*> vertices;
    vector<Edge*> edges;
    string tmp;
    vector<string> lines;
    while (getline(fin, tmp)) {
        lines.push_back(tmp);
    }
    for (size_t i=0;i<lines.size();i++) {
        istringstream ss(lines[i]);
        int l;
        ss >> l;
        vertices.push_back(new Vertex(l));
    }
    for (size_t i=0;i<lines.size();i++) {
        istringstream ss(lines[i]);
        int l;
        ss >> l;
        int t, w;
        char c;
        while (ss >> t >> c >> w) {
            vertices[i]->target.push_back(new Edge(vertices[i], vertices[t-1], w));
        }
    }
    return vertices;
}

void dijkstra(VertexQueue &vqueue) {
    while (vqueue.size() > 0) {
        Vertex *current = vqueue.pop_front();
        current->marked = true;
        for (size_t i=0;i<current->target.size();i++) {
            if (current->target[i]->to->marked)
                continue;
            if (current->target[i]->to->queue_idx == -1)
                vqueue.insert(current->target[i]->to, current->dis + current->target[i]->weight);
            else
                vqueue.decrement(current->target[i]->to->queue_idx, current->dis + current->target[i]->weight);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "There should be exactly one parameter!" << endl;
        return 0;
    }
    ifstream fin(argv[1]);
    if (!fin.is_open()) {
        cout << argv[1] << " is not a valid file (path)" << endl;
        return 0;
    }

    vector<Vertex*> vertices = data_process(fin);

    VertexQueue vqueue;
    vqueue.insert(vertices[0], 0);
    //vqueue.display();
    dijkstra(vqueue);

    int indices[] = {7,37,59,82,99,115,133,165,188,197};
    for (size_t i=0;i<10;i++) {
        cout << *(vertices[indices[i]-1]) << endl;
    }
    return 0;
}
