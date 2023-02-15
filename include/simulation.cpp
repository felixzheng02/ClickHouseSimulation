#include "simulation.hpp"


Simulation::Simulation() {
    queryGenerator = new QueryGenerator(2.0, 1.0);
};

void Simulation::initialize() {
    time_a = queryGenerator->next().arrival;
}

double Simulation::run() {
    for (int i=0; i<5; i++) {
        Query query = queryGenerator->next();
        time += query.size;
    }
    return time;
}
