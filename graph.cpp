#include "graph.h"
#include <iostream>

// Adds an airport to the graph (vertex object)
void Graph::addAirport(const std::string& code) {
    for (const auto& vertex : vertices) { // Check to see if airport code already exists in graph
        if (vertex.airportCode == code) return;
    }
    // Create new vertex, set airport code, and add it to vertices vector
    Vertex newVertex;
    newVertex.airportCode = code;
    vertices.push_back(newVertex);
}

// Adds a flight to the graph (edge object)
void Graph::addFlight(const std::string& origin, const std::string& dest, int distance, int cost) {
    int originIndex = getAirportIndex(origin);
    int destIndex = getAirportIndex(dest);
    if (originIndex == -1 || destIndex == -1) return;

    // Create new edge, set variables, add to a vertex adjacencyList based on given origin
    Edge e;
    e.destIndex = destIndex;
    e.distance = distance;
    e.cost = cost;
    vertices[originIndex].adjacencyList.push_back(e);
}

// Attempts to find and return the index of the given airport code in the vertices vector. If not found, returns -1
int Graph::getAirportIndex(const std::string& code) const {
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i].airportCode == code) return i;
    }
    return -1; // Not found
}

// Prints graph for debugging
void Graph::printGraph() const {
    for (const auto& vertex : vertices) {
        std::cout << "Airport: " << vertex.airportCode << std::endl;
        for (const auto& edge : vertex.adjacencyList) {
            std::cout << "  -> " << vertices[edge.destIndex].airportCode
                      << " (Dist: " << edge.distance << ", Cost: " << edge.cost << ")" << std::endl;
        }
    }
}

// Returns graph vertices
const std::vector<Graph::Vertex>& Graph::getVertices() const {
    return vertices;
}