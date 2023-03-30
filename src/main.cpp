#include "../include/simulation.hpp"
#include <iostream>

int ExpExp(int n_cores, double arrival_lambda, double phase_size_lambda, int n_iteration);


std::string getText(Policy policy) {
    switch (policy) {
        case FCFS:
            return "FCFS";
        case SJF:
            return "SJF";
        case SRPT_query:
           return "SRPT_query";
        case SRPT_phase:
           return "SRPT_phase";
    }
}

bool FCFSCompare(Query *query_1, Query *query_2) {
    return query_1->arrival < query_2->arrival;
}

int main() {

    int n_cores = 64;
    std::vector<Policy> policies = {FCFS, SJF, SRPT_query, SRPT_phase};
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

        auto compare_func = (policy == FCFS) ? compare_func_arrival : (policy == SJF) ? compare_func_size : (policy == SRPT_query) ? compare_func_size : (policy == SRPT_phase) ? compare_func_size : compare_func_arrival;

        for (double size=1.1; size<100; size+=0.1) {

            ExponentialDistribution arrival_dist(arrival_lambda);
            ExponentialDistribution phase_size_dist(1/size);
            QueryGenerator query_generator(&arrival_dist, &phase_size_dist);
                       
            Simulation simulation(n_cores, policy, compare_func, query_generator);
            simulation.initialize();

            for (int i=0; i<1000; i++) {
                simulation.run();
            }

            data << size << ", " << simulation.getMeanJobs() << ", \n";

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
