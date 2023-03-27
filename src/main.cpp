#include "../include/simulation.hpp"
#include <iostream>

int ExpExp(int n_cores, double arrival_lambda, double phase_size_lambda, int n_iteration);


std::string getText(Policy policy) {
    switch (policy) {
        case FCFS:
            return "FCFS";
        case SJF:
            return "SJF";
        case SRPT:
           return "SRPT";
    }
}

bool FCFSCompare(Query *query_1, Query *query_2) {
    return query_1->arrival < query_2->arrival;
}

int main() {

    int n_cores = 8;
    std::vector<Policy> policies = {FCFS, SJF, SRPT};
    double arrival_lambda = 2;
    
    for (Policy policy : policies) {
        std::ofstream data;
        data.open ("../data/exp_exp_" + getText(policy) + ".csv");
        data << "phase_size, mean_jobs, \n";

        auto compare_func_arrival = [](Query *query_1, Query *query_2) {
            return query_1->arrival < query_2->arrival;
        };

        auto compare_func_size = [](Query *query_1, Query *query_2) {
            return query_1->size < query_2->size;
        };

        auto compare_func = (policy == FCFS) ? compare_func_arrival : (policy == SJF) ? compare_func_size : (policy == SRPT) ? compare_func_size : compare_func_arrival;

        for (double size_lambda=0.1; size_lambda<4; size_lambda+=0.05) {

            ExponentialDistribution arrival_dist(arrival_lambda);
            ExponentialDistribution phase_size_dist(size_lambda);
            QueryGenerator query_generator(&arrival_dist, &phase_size_dist);
                       
            Simulation simulation(n_cores, policy, compare_func, query_generator);
            simulation.initialize();

            for (int i=0; i<1000; i++) {
                simulation.run();
            }

            data << size_lambda << ", " << simulation.getMeanJobs() << ", \n";

            // ExpExp(n_cores, arrival_lambda, phase_size_lambda - i, 100);
        }

        data.close();
    }

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
