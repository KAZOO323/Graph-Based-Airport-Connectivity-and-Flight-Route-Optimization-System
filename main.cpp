#include <fstream>
#include <sstream>
#include <iostream>
#include "graph.h"
#include "airport.h"
#include <string>

// Loads a given CSV file into a given graph object
void loadCSV(const std::string& filename, Graph& g) {
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

        getline(ss, origin, ',');
        getline(ss, dest, ',');
        getline(ss, originCity, ',');
        getline(ss, destCity, ',');
        getline(ss, distStr, ',');
        getline(ss, costStr, ',');

        int distance = std::stoi(distStr);
        int cost = std::stoi(costStr);

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

    loadCSV("airports.csv", g);

    g.printGraph();

    return 0;
}
