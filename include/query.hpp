#include <vector>
#include "phase.hpp"

struct Query {
    std::vector <Phase> phases;
    double arrival; // arrival time
    double size; // remaining processing time
    double memory = 0; // amount of memory required
};
