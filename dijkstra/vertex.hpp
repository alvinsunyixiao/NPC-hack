#ifndef _VERTEX_H_
# define _VERTEX_H_

#include <vector>
#include <string>

using namespace std;

struct Position {
    int x;
    int y;
};

class Edge;

class Vertex {
public:
    string          label;
    bool            marked;
    double          dis;
    int             queue_idx;
    Position        *pos;
    Vertex          *parent;
    vector<Edge*>   target;

    Vertex(Position *p, string label);
    void display();
    bool operator<(Vertex &v);
    bool operator>(Vertex &v);
    friend ostream &operator<<(ostream &output, Vertex &v);
};

#endif
