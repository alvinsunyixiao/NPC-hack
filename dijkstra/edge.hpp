#ifndef _EDGE_H_
# define _EDGE_H_

#include <vector>

class Vertex;

class Edge {
public:
    Vertex  *from;
    Vertex  *to;
    double  weight;

    Edge(Vertex *f, Vertex *t);  //Construct an Edge instance from
    double calc_dis();
};

#endif
