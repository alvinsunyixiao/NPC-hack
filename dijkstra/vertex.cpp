#include <iostream>
#include <string>
#include "vertex.hpp"
#include "edge.hpp"

using namespace std;

Vertex::Vertex(Position *p, string l) {
    marked = false;
    label = l;
    dis = 100000000;
    queue_idx = -1;
    pos = p;
    parent = 0;
}

bool Vertex::operator<(Vertex &v) {
    return dis < v.dis;
}

bool Vertex::operator>(Vertex &v) {
    return dis > v.dis;
}

void Vertex::display() {
    cout << "Label: " << label << endl;
    cout << "Position: (" << pos->x << ", " << pos->y << ")" << endl;
    cout << "Marked: " << marked << endl;
    cout << "Distance: " << dis << endl;
    cout << "Queue Index: " << queue_idx << endl;
    cout << "Target: [ ";
    for (size_t i=0;i<target.size();i++) {
        cout << "(" << target[i]->to->pos->x << ", " << target[i]->to->pos->y << ", " << target[i]->to->label << ')' << ' ';
    }
    cout << "]" << endl;
    cout << endl;
}

ostream &operator<<(ostream &output, Vertex &v) {
    output << "Label: " << v.label << endl;
    output << "Position: (" << v.pos->x << ", " << v.pos->y << ")" << endl;
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
