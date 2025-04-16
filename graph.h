#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>

class Graph {
private:
    struct Edge { // Represents direct flights between airports
        int destIndex;
        int distance;
        int cost;
    };

    struct Vertex { // Represents airports
        std::string airportCode;
        std::vector<Edge> adjacencyList;
    };
    
    // Vector containing graph vertex objects
    std::vector<Vertex> vertices;

public: // See implementation file for details
    void addAirport(const std::string& code);
    void addFlight(const std::string& origin, const std::string& dest, int distance, int cost);
    int getAirportIndex(const std::string& code) const;
    void printGraph() const;
    const std::vector<Vertex>& getVertices() const;
};

#endif