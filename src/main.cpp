#include "../include/simulation.hpp"
#include <iostream>


int ExpExp(int n_cores, double arrival_lambda, double phase_size_lambda, int n_iteration);


std::string getText(Policy policy) {
    switch (policy) {
       case FCFS:
           return "FCFS";
       case SJF:
           return "SJF";
    }
}


int main() {

    int n_cores = 8;
    Policy policy = FCFS;
    double arrival_lambda = 1;
    
    std::ofstream data;
    data.open ("../data/exp_exp_" + getText(policy) + ".csv");
    data << "phase_size, mean_jobs, \n";

    for (double size=0.1; size<3.5; size+=0.1) {

        Distribution<double> *arrival_dist = new ExponentialDistribution(arrival_lambda);
        Distribution<double> *phase_size_dist = new ExponentialDistribution(1/size);
        QueryGenerator *query_generator = new QueryGenerator(arrival_dist, phase_size_dist);

        Simulation *simulation = new Simulation(n_cores, policy, query_generator);
        simulation->initialize();

        for (int i=0; i<2000; i++) {
            simulation->run();
        }

        data << size << ", " << simulation->getMeanJobs() << ", \n";

        delete simulation;
        // ExpExp(n_cores, arrival_lambda, phase_size_lambda - i, 100);
    }

    data.close();

    return 1;
}


/*
int ExpExp(int n_cores, double arrival_lambda, double phase_size_lambda, int n_iteration) {

    Distribution<double> *arrival_dist = new ExponentialDistribution(arrival_lambda);
    Distribution<double> *phase_size_dist = new ExponentialDistribution(phase_size_lambda);
    QueryGenerator *query_generator = new QueryGenerator(arrival_dist, phase_size_dist);

    Simulation *simulation = new Simulation(n_cores, query_generator);
    simulation->initialize();

    for (int i=0; i<n_iteration; i++) {
        simulation->run();
    }

    std::cout << "mean jobs: " << simulation->getMeanJobs() << std::endl;

    delete simulation;

    return 1;
}
*/
