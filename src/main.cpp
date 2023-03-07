#include "../include/simulation.hpp"
#include <iostream>

int main() {

    Distribution<double> *arrival_dist = new ExponentialDistribution(1.0);
    Distribution<double> *phase_size_dist = new UniformDistribution(0.0, 5.0);
    QueryGenerator *query_generator = new QueryGenerator(arrival_dist, phase_size_dist);

    Simulation *simulation = new Simulation(8, query_generator);
    simulation->initialize();

    for (int i=0; i<10; i++) {
        simulation->run();
    }

    return 1;
}
