#include "graph.h"
#include <iostream>
using namespace std;

// Queue methods
void Queue::push(int index, int distance, int cost, int stops, const std::vector<int>& path) {
    data.push_back({index, distance, cost, stops, path});
}

Queue::Node Queue::pop() {
    Node node = data.front();
    data.erase(data.begin());
    return node;
}

bool Queue::empty() const {
    return data.empty();
}

// Graph methods
// Adds an airport to the graph (vertex object)
void Graph::addAirport(const std::string& code, const std::string& state) {
    for (const auto& vertex : vertices) { // Check to see if airport code already exists in graph
        if (vertex.airportCode == code) return;
    }
    
    // Create new vertex, set airport code & state, and add it to vertices vector
    Vertex newVertex;
    newVertex.airportCode = code;
    newVertex.state = state;
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
        std::cout << "Airport: " << vertex.airportCode << ", State: " << vertex.state << std::endl;
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

// Calculates and prints the shortest path between the given origin and destination airports (dijkstra's algorithm)
void Graph::shortestPath(const std::string& origin, const std::string& destination) const {
    // Get source and destination airport index
    int i_src = getAirportIndex(origin);
    int i_dest = getAirportIndex(destination);
    
    // Index validation
    if (i_src == -1 || i_dest == -1) {
        throw std::string("Shortest path: incorrect vertices"); 
    }

    // Initial distances
    std::vector<int> distances(vertices.size(), std::numeric_limits<int>::max());
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> prev(vertices.size(), -1);
    distances[i_src] = 0;
    int cur = i_src;
    int visited_count = 0;

    // Visit all vertices
    while (visited_count < vertices.size()) {
        int i = cur;

        for (const auto& edge : vertices[i].adjacencyList) {
            int neighbor = edge.destIndex;
            if (!visited[neighbor]) {
                int alt = distances[i] + edge.distance;
                if (alt < distances[neighbor]) {
                    distances[neighbor] = alt;
                    prev[neighbor] = i;
                }
            }
        }

        // Find the next closest unvisited vertex
        int minDist = std::numeric_limits<int>::max();
        cur = -1;
        for (int j = 0; j < vertices.size(); j++) {
            if (!visited[j] && distances[j] < minDist) {
                minDist = distances[j];
                cur = j;
            }
        }

        if (cur == -1) break; // No valid vertex, break loop

        visited[cur] = true;
        visited_count++;
    }

    // No path available
    if (distances[i_dest] == std::numeric_limits<int>::max()) {
        std::cout << "Shortest route from " << origin << " to " << destination << ": " << "N/A" << std::endl;
        return;
    }

    // Retrace path
    std::vector<int> path;
    for (int at = i_dest; at != -1; at = prev[at]) {
        path.insert(path.begin(), at);
    }
    
    // Calculate cost
    int routeCost = 0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];
        for (const auto& edge : vertices[from].adjacencyList) {
            if (edge.destIndex == to) {
                routeCost += edge.cost;
                break;
            }
        }
    }
    
    // Output shortest route
    std::cout << "Shortest route from " << origin << " to " << destination << ": ";
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << vertices[path[i]].airportCode;
        if (i != path.size() - 1) std::cout << " -> ";
    }
    // Output distance and cost
    std::cout << ". The length is " << distances[i_dest] << ".";
    std::cout << " The cost is " << routeCost << "." << std::endl;
}

// Calculates and prints all shortest paths between the given origin airport and all destination airports in a given state (dijkstra's algorithm)
void Graph::shortestPathsToState(const std::string& origin, const std::string& state) const {
    // Get source airport index
    int src = getAirportIndex(origin);
    
    // Index validation
    if (src == -1) {
        std::cout << "Invalid origin airport." << std::endl;
        return;
    }
    
    // Initial distances
    std::vector<int> distances(vertices.size(), std::numeric_limits<int>::max());
    std::vector<bool> visited(vertices.size(), false);
    std::vector<int> prev(vertices.size(), -1);
    distances[src] = 0;
    int cur = src;
    int visited_count = 0;
    
    // Visit all vertices
    while (visited_count < vertices.size()) {
        int i = cur;

        for (const auto& edge : vertices[i].adjacencyList) {
            int neighbor = edge.destIndex;
            if (!visited[neighbor]) {
                int alt = distances[i] + edge.distance;
                if (alt < distances[neighbor]) {
                    distances[neighbor] = alt;
                    prev[neighbor] = i;
                }
            }
        }
        
        // Find the next closest unvisited vertex
        int minDist = std::numeric_limits<int>::max();
        cur = -1;
        for (int j = 0; j < vertices.size(); j++) {
            if (!visited[j] && distances[j] < minDist) {
                minDist = distances[j];
                cur = j;
            }
        }

        if (cur == -1) break; // No valid vertex, break loop

        visited[cur] = true;
        visited_count++;
    }
    
    // Print routes
    std::cout << "Shortest path from " << origin << " to " << state << " state airports are:" << endl;
    bool pathFound = false;
    for (int dst = 0; dst < vertices.size(); ++dst) {
        if (distances[dst] == std::numeric_limits<int>::max()) continue;

        if (vertices[dst].state != state) continue;

        pathFound = true;
        std::vector<int> path;
        for (int at = dst; at != -1; at = prev[at]) {
            path.insert(path.begin(), at);
        }
        
        // Calculate route cost
        int totalCost = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int from = path[i];
            int to = path[i + 1];
            for (const auto& edge : vertices[from].adjacencyList) {
                if (edge.destIndex == to) {
                    totalCost += edge.cost;
                    break;
                }
            }
        }
        
        // Print route
        for (size_t j = 0; j < path.size(); ++j) {
            std::cout << vertices[path[j]].airportCode;
            if (j != path.size() - 1) std::cout << " -> ";
        }
        // Print distance and cost of route
        std::cout << " | Length = " << distances[dst] << ", Cost = " << totalCost << std::endl;
    }
    
    // No paths found
    if (!pathFound) {
        std::cout << "N/A" << std::endl;
    }
}

// Calculates and prints the shortest path between the given origin and destination airport with a specified amount of stops
void Graph::shortestPathWithStops(const std::string& origin, const std::string& destination, int stops) const {
    // Get source and destination airport index
    int src = getAirportIndex(origin);
    int dst = getAirportIndex(destination);
    // Index validation
    if (src == -1 || dst == -1) {
        std::cout << "Invalid airport codes." << std::endl;
        return;
    }
    
    // BFS node structure
    struct Node {
        int index;
        int distance;
        int cost;
        int stops;
        std::vector<int> path;
    };
    // Create and prepare queue, path, cost
    std::vector<Node> queue;
    queue.push_back({src, 0, 0, 0, {src}});
    int minDistance = std::numeric_limits<int>::max();
    std::vector<int> Path;
    int Cost = 0;
    
    // Populate path (BFS Traversal)
    while (!queue.empty()) {
        // Dequeue front
        Node node = queue.front();
        queue.erase(queue.begin());
        
        // Destination reached with stops requirement
        if (node.index == dst && node.stops == stops+1) { // stops+1 is a really hacky bandage for my awful logic but it functions
            if (node.distance < minDistance) {
                minDistance = node.distance;
                Path = node.path;
                Cost = node.cost;
            }
            continue; // path found
        }
        
        if (node.stops >= stops+1) continue;
        
        // Traverse adjacent
        for (const auto& edge : vertices[node.index].adjacencyList) {
            std::vector<int> newPath = node.path;
            newPath.push_back(edge.destIndex);
            queue.push_back({
                edge.destIndex, 
                node.distance + edge.distance, 
                node.cost + edge.cost, 
                node.stops + 1, 
                newPath
                
            });
        }
    }

    // Print results
    std::cout << "Shortest path from " << origin << " to " << destination << " with " << stops << " stops: ";
    
    // No path found
    if (Path.empty()) {
        std::cout << "N/A" << std::endl;
        return;
    }
    
    // Path found; print path
    for (size_t i = 0; i < Path.size(); ++i) {
        std::cout << vertices[Path[i]].airportCode;
        if (i != Path.size() - 1) std::cout << " -> ";
    }
    std::cout << ". The length is " << minDistance << ". The cost is " << Cost << std::endl;
}

// Gathers and prints total direct flight connections for each airport, descending order
void Graph::printFlightConnections() const {
    std::vector<int> inboundCount(vertices.size(), 0); // Inbound connections
    std::vector<std::pair<std::string, int>> connectionData; // Airport codes & their connections
    
    // Calculate inbound connections
    for (int i = 0; i < vertices.size(); ++i) {
        for (int j = 0; j < vertices[i].adjacencyList.size(); ++j) {
            int dest = vertices[i].adjacencyList[j].destIndex;
            if (dest >= 0 && dest < vertices.size()) {
                inboundCount[dest]++;
            }
        }
    }
    
    // Calculate outbound connections, add to inbound for total
    for (int i = 0; i < vertices.size(); ++i) {
        int outbound = vertices[i].adjacencyList.size();
        int inbound = inboundCount[i];
        int total = outbound + inbound;
        connectionData.push_back({vertices[i].airportCode, total});
    }

    // Sort descending
    for (int i = 0; i < connectionData.size() - 1; ++i) {
        int maxIdx = i;
        for (int j = i + 1; j < connectionData.size(); ++j) {
            if (connectionData[j].second > connectionData[maxIdx].second) {
                maxIdx = j;
            }
        }
        if (maxIdx != i) {
            std::pair<std::string, int> temp = connectionData[i];
            connectionData[i] = connectionData[maxIdx];
            connectionData[maxIdx] = temp;
        }
    }
    
    // Print each airport and their total connections
    std::cout << "Airport | Connections" << std::endl;
    std::cout << "---------------------" << std::endl;
    for (int i = 0; i < connectionData.size(); ++i) {
        std::cout << connectionData[i].first << "     | " << connectionData[i].second << std::endl;
    }
}

// Undirected Graph methods
// Constructor
Graph Graph::createUndirectedGraph() const {
    Graph undirectedGraph;

    // Copy vertexes (airports) to new graph
    for (const auto& vertex : vertices) {
        undirectedGraph.addAirport(vertex.airportCode, vertex.state);
    }
    
    // Iterate through edges (flights)
    for (int u = 0; u < vertices.size(); ++u) {
        for (const auto& edge : vertices[u].adjacencyList) {
            int v = edge.destIndex;
            
            // Look for reverse edge
            int reverseCost = -1;
            for (const auto& revEdge : vertices[v].adjacencyList) {
                if (revEdge.destIndex == u) {
                    reverseCost = revEdge.cost;
                    break;
                }
            }

            if (u < v) { // Avoid duplicates
                // Use smaller cost as undirected edge, otherwise just use edge.cost
                int minCost = (reverseCost != -1) ? std::min(edge.cost, reverseCost) : edge.cost;
                undirectedGraph.addFlight(vertices[u].airportCode, vertices[v].airportCode, 0, minCost);
                undirectedGraph.addFlight(vertices[v].airportCode, vertices[u].airportCode, 0, minCost);
            }
        }
    }

    return undirectedGraph;
}

// Creates an MST using Prim's algorithm on an undirected graph.
// Note: The given airport data always creates a disconnected graph, meaning this will never actually work.
void Graph::primMST() const {
    // Empty graphs can't make MSTs
    if (vertices.empty()) {
        std::cout << "Graph is empty. MST cannot be formed." << std::endl;
        return;
    }

    int n = vertices.size();
    std::vector<bool> inMST(n, false); // Track presence in MST
    std::vector<int> parent(n, -1); // Store parents of each vertex
    std::vector<int> key(n, std::numeric_limits<int>::max()); // Minimum cost for each vertex
    key[0] = 0;
    
    // Construct MST
    for (int count = 0; count < n - 1; ++count) {
        int u = -1;
        int minKey = std::numeric_limits<int>::max();
        for (int v = 0; v < n; ++v) { // Prioritize smaller keys
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        if (u == -1) { // Graph is disconnected, can't build MST
            std::cout << "Disconnected graph; primMST cannot be built" << std::endl;
            return;
        }

        inMST[u] = true; // Mark selected vertex as being in MST
        
        // Update key and parent for adj.
        for (const auto& edge : vertices[u].adjacencyList) {
            int v = edge.destIndex;
            if (!inMST[v] && edge.cost < key[v]) {
                key[v] = edge.cost;
                parent[v] = u;
            }
        }
    }
    
    // Print MST
    int totalCost = 0;
    std::cout << "Minimal Spanning Tree (Prim): " << std::endl;
    for (int i = 1; i < n; ++i) {
        if (parent[i] != -1) {
            std::cout << vertices[parent[i]].airportCode << " - " << vertices[i].airportCode << " | Weight: " << key[i] << std::endl;
            totalCost += key[i];
        }
    }
    std::cout << "Total cost of MST: " << totalCost << std::endl;
}

// Creates an MST using Kruskal's algorithm on an undirected graph.
void Graph::kruskalMST() const {
    struct EdgeInfo { // Helper structure
        int u;
        int v;
        int cost;
    };
    
    // Get all edges
    std::vector<EdgeInfo> allEdges;
    for (int u = 0; u < vertices.size(); ++u) {
        for (const auto& edge : vertices[u].adjacencyList) {
            if (u < edge.destIndex) {
                allEdges.push_back({u, edge.destIndex, edge.cost});
            }
        }
    }

    // Sort edges by cost
    for (int i = 0; i < allEdges.size() - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < allEdges.size(); ++j) {
            if (allEdges[j].cost < allEdges[minIdx].cost) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            EdgeInfo temp = allEdges[i];
            allEdges[i] = allEdges[minIdx];
            allEdges[minIdx] = temp;
        }
    }
    
    // Union-find
    std::vector<int> parent(vertices.size());
    for (int i = 0; i < vertices.size(); ++i) {
        parent[i] = i;
    }
    
    // Find set root (helper)
    auto find = [&](int x) {
        while (x != parent[x]) x = parent[x];
        return x;
    };
    
    // Unite sets (helper)
    auto unite = [&](int x, int y) {
        int rootX = find(x);
        int rootY = find(y);
        if (rootX == rootY) return false;
        parent[rootY] = rootX;
        return true;
    };
    
    // Print MST
    int totalCost = 0;
    std::cout << "Minimal Spanning Tree (Kruskal): " << std::endl;
    for (const auto& edge : allEdges) {
        if (unite(edge.u, edge.v)) {
            std::cout << vertices[edge.u].airportCode << " - " << vertices[edge.v].airportCode << " | Weight: " << edge.cost << std::endl;
            totalCost += edge.cost;
        }
    }
    std::cout << "Total cost of MST: " << totalCost << std::endl;
}