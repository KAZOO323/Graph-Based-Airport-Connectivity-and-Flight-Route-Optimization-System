#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>

struct Airport {
    std::string code;
    std::string city;
    std::string state;
};

struct Flight {
    std::string origin;
    std::string destination;
    int distance;
    int cost;
};

#endif