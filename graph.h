#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <limits>

class Graph {
private:
    struct Edge { // Represents direct flights between airports
        int destIndex;
        int distance;
        int cost;
    };

    struct Vertex { // Represents airports
        std::string airportCode;
        std::string city;
        std::string state;
        std::vector<Edge> adjacencyList;
    };
    
    // Vector containing graph vertex objects
    std::vector<Vertex> vertices;

public: // See implementation file for details
    void addAirport(const std::string& code, const std::string& state);
    void addFlight(const std::string& origin, const std::string& dest, int distance, int cost);
    int getAirportIndex(const std::string& code) const;
    void printGraph() const;
    const std::vector<Vertex>& getVertices() const;
    void shortestPath(const std::string& origin, const std::string& destination) const;
    void shortestPathsToState(const std::string& origin, const std::string& state) const;
};

#endif