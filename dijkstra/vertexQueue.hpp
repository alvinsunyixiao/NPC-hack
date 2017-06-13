#ifndef _VERTEXQUEUE_H_
# define _VERTEXQUEUE_H_

#include "edge.hpp"
#include "vertex.hpp"
#include <vector>

using namespace std;

class VertexQueue {
public:
    vector<Vertex*> data;

    VertexQueue();
    void    heapify(size_t i);
    void    insert(Edge *e);
    void    decrement(size_t i, Edge *e);
    void    display();
    size_t  size();
    Vertex* pop_front();
};

#endif
