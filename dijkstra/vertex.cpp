#include <iostream>
#include <string>
#include "vertex.hpp"
#include "edge.hpp"

using namespace std;

Vertex::Vertex(int x, int y, string l) {
    marked = false;
    label = l;
    dis = 100000000;
    queue_idx = -1;
    pos = {x, y};
}

bool Vertex::operator<(Vertex &v) {
    return dis < v.dis;
}

bool Vertex::operator>(Vertex &v) {
    return dis > v.dis;
}

ostream &operator<<(ostream &output, Vertex &v) {
    output << "Label: " << v.label << endl;
    output << "Position: (" << v.pos.x << ", " << v.pos.y << ")" << endl;
    output << "Marked: " << v.marked << endl;
    output << "Distance: " << v.dis << endl;
    output << "Queue Index: " << v.queue_idx << endl;
    output << "Target: [ ";
    for (size_t i=0;i<v.target.size();i++) {
        output << v.target[i]->to->label << ' ';
    }
    output << "]" << endl;
    return output;
}
