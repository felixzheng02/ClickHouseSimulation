#include "../include/simulation.hpp"
#include <iostream>

int main() {

    Distribution<double> *arrival_dist = new ExponentialDistribution(1.0);
    Distribution<double> *phase_size_dist = new ExponentialDistribution(2);
    QueryGenerator *query_generator = new QueryGenerator(arrival_dist, phase_size_dist);

    Simulation *simulation = new Simulation(8, query_generator);
    simulation->initialize();

    for (int i=0; i<100; i++) {
        simulation->run();
    }

    std::cout << "mean jobs: " << simulation->getMeanJobs() << std::endl;

    return 1;
}
