#include <vector>
#include <iostream>

#include "edge.hpp"
#include "vertex.hpp"
#include "vertexQueue.hpp"

using namespace std;

VertexQueue::VertexQueue() {}

void VertexQueue::decrement(size_t i, Edge *e) {
    if (data[i]->dis <= e->from->dis + e->weight)
        return;
    data[i]->dis = e->from->dis + e->weight;
    e->to->parent = e->from;
    int p = (i-1) / 2;
    while (p >= 0 && *(data[i]) < *(data[p])) {
        swap(data[i], data[p]);
        data[i]->queue_idx = i;
        data[p]->queue_idx = p;
        i = p;
        p = (i-1) / 2;
    }
}

void VertexQueue::insert(Edge *e) {
    data.push_back(e->to);
    e->to->queue_idx = data.size() - 1;
    decrement(data.size()-1, e);
}

void VertexQueue::display() {
    cout << "-----------Head of Queue-----------" << endl;
    for (size_t i=0;i<data.size();i++) {
        cout << *(data[i]) << endl;
    }
    cout << "------------End of Queue-----------" << endl << endl;
}

void VertexQueue::heapify(size_t i) {
    size_t min_idx = i;
    if (i*2+1 < data.size() && *(data[i*2+1]) < *(data[min_idx]))
        min_idx = i*2+1;
    if (i*2+2 < data.size() && *(data[i*2+2]) < *(data[min_idx]))
        min_idx = i*2+2;
    if (min_idx != i) {
        swap(data[i], data[min_idx]);
        data[i]->queue_idx = i;
        data[min_idx]->queue_idx = min_idx;
        heapify(min_idx);
    }
    return;
}

size_t VertexQueue::size() {
    return data.size();
}

Vertex* VertexQueue::pop_front() {
    Vertex *rs = data[0];
    swap(data[0], data[data.size()-1]);
    data[0]->queue_idx = 0;
    data.pop_back();
    heapify(0);
    return rs;
}
