#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

class Graph {
private:
    struct Edge {
        int destIndex;
        int distance;
        int cost;
    };

    struct Vertex {
        std::string airportCode;
        std::vector<Edge> adjacencyList;
    };

    std::vector<Vertex> vertices;

public:
    
};

#endif