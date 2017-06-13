#include <math.h>
#include "edge.hpp"
#include "vertex.hpp"

using namespace std;

Edge::Edge(Vertex *f, Vertex *t) {
    from = f;
    to = t;
    weight = calc_dis();
}

double Edge::calc_dis() {
    return sqrt((from->pos.x - to->pos.x)^2 + (from->pos.y - to->pos.y)^2);
}
