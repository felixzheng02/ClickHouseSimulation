#include "../include/simulation.hpp"
#include <iostream>
#include <memory>


std::string getText(Policy policy) {
    switch (policy) {
        case RR:
            return "RR";
        case SRPT_query:
           return "SRPT_query";
        case NEW_1:
           return "NEW_1";
    }
}


int main() {

    int n_cores = 64;
    std::vector<Policy> policies = {SRPT_query};
    double arrival_lambda = 1;
    
    for (Policy policy : policies) {
        std::ofstream data;
        data.open ("../data/exp_exp_" + getText(policy) + ".csv");
        data << "phase_size, mean_jobs, \n";

        auto compare_func_arrival = [](std::shared_ptr<Query> query_1, std::shared_ptr<Query> query_2) {
            return query_1->arrival < query_2->arrival;
        };

        auto compare_func_size = [](std::shared_ptr<Query> query_1, std::shared_ptr<Query> query_2) {
            return query_1->size < query_2->size;
        };

        auto compare_func = (policy == SRPT_query) ? compare_func_size : (policy == NEW_1) ? compare_func_size : compare_func_arrival;

        for (double size=1; size<=20; size+=5) {

            ExponentialDistribution arrival_dist(arrival_lambda);
            ParetoDistribution phase_size_dist(1.5, size);
            QueryGenerator query_generator(&arrival_dist, &phase_size_dist, n_cores);
                       
            int iteration = 100; 
            double mean_jobs = 0;
            for (int i=0; i<=(iteration-1); i++) {
                Simulation simulation(n_cores, policy, compare_func, query_generator);
                simulation.initialize();
                for (int j=0; j<10000; j++) {
                    simulation.run();
                }
                mean_jobs += simulation.getMeanJobs();
            }
            mean_jobs = mean_jobs/iteration;

            data << size << ", " << mean_jobs << ", \n";

            // ExpExp(n_cores, arrival_lambda, phase_size_lambda - i, 100);
        }

        data.close();
    }

    return 1;
}
