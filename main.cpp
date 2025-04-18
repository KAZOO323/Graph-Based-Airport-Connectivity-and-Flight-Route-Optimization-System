#include <fstream>
#include <sstream>
#include <iostream>
#include "graph.h"
#include "airport.h"
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
        std::string origin, dest, originCity, destCity, distStr, costStr;
        
        // Gather variable data
        getline(ss, origin, ',');
        getline(ss, dest, ',');
        
        // Handle city strings properly
        if (ss.peek() == '"') ss.ignore();
        getline(ss, originCity, '"');
        ss.ignore(1, ',');
        
        if (ss.peek() == '"') ss.ignore();
        getline(ss, destCity, '"');
        ss.ignore(1, ',');

        getline(ss, distStr, ',');
        getline(ss, costStr, ',');
        
        int distance = std::stoi(distStr);
        int cost = std::stoi(costStr);
        
        // Add airports and flights
        g.addAirport(origin);
        g.addAirport(dest);
        g.addFlight(origin, dest, distance, cost);
    }
    
    // Close file
    file.close();
}

// Main function
int main() {
    Graph g;

    loadTXT("airports.txt", g);

    g.printGraph();
    std::cout << std::endl;
    g.shortestPath("ATL", "AFW");

    return 0;
}
