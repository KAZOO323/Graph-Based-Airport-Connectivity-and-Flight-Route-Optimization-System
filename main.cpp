#include <fstream>
#include <sstream>
#include <iostream>
#include "graph.h"
#include <string>

// Loads a given text file into a given graph object. OnlineGDB does not recognize .csv files, so this will have to do.
void loadTXT(const std::string& filename, Graph& g) {
    // Open file
    std::ifstream file(filename);
    // Ensure file is actually open
    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }
    
    // Define line object
    std::string line;
    getline(file, line);
    
    // Iterate through file lines and add airports & graphs
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string origin, dest, originCity, originState, destCity, destState, distStr, costStr;
        
        // Gather variable data
        getline(ss, origin, ',');
        getline(ss, dest, ',');
        
        // Handle city strings properly
        if (ss.peek() == '"') ss.ignore();
        getline(ss, originCity, ',');
        
        if (ss.peek() == ' ') ss.ignore();
        getline(ss, originState, '"');
        
        ss.ignore(1, ',');
        
        if (ss.peek() == '"') ss.ignore();
        getline(ss, destCity, ',');
        
        if (ss.peek() == ' ') ss.ignore();
        getline(ss, destState, '"');
        
        ss.ignore(1, ',');

        getline(ss, distStr, ',');
        getline(ss, costStr, ',');
        
        int distance = std::stoi(distStr);
        int cost = std::stoi(costStr);
        
        // Add airports and flights
        g.addAirport(origin, originState);
        g.addAirport(dest, destState);
        g.addFlight(origin, dest, distance, cost);
    }
    
    // Close file
    file.close();
}

// Main function
int main() {
    Graph g;

    loadTXT("airports.txt", g); // 1)

    g.printGraph();
    std::cout << std::endl;
    g.shortestPath("ATL", "AFW"); // 2)
    std::cout << std::endl;
    g.shortestPathsToState("ATL", "FL"); // 3)
    std::cout << std::endl;
    g.shortestPathWithStops("IAD", "MIA", 3); // 4)
    std::cout << std::endl;
    g.printFlightConnections(); // 5)
    std::cout << std::endl;
    
    Graph g_u = g.createUndirectedGraph(); // 6)
    
    g_u.primMST(); // 7)
    std::cout << std::endl;
    g_u.kruskalMST(); // 8)
    
    return 0;
}
