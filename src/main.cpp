#include "../include/simulation.hpp"
#include <iostream>
#include <memory>
#include <map>


std::string getText(Policy policy) {
    switch (policy) {
        case FCFS:
            return "FCFS";
        case RR:
            return "RR";
        case SRPT_query:
           return "SRPT_query";
        case NEW_1:
           return "NEW_1";
    }
}
// speed up S
// E[slow down] = E[T/S]

int simulation() {
    int n_cores = 512;
    std::vector<Policy> policies = {SRPT_query, RR, FCFS};
    double arrival_lambda = 1;
    
    for (Policy policy : policies) {
        std::ofstream data;
        data.open ("../data/exp_exp_" + getText(policy) + ".csv");
        data << "arrival_lambda, mean_jobs, expected_slowdown, \n";

        auto compare_func_arrival = [](std::shared_ptr<Query> query_1, std::shared_ptr<Query> query_2) {
            return query_1->arrival < query_2->arrival;
        };

        auto compare_func_size = [](std::shared_ptr<Query> query_1, std::shared_ptr<Query> query_2) {
            return query_1->size < query_2->size;
        };

        auto compare_func = (policy == FCFS) ? compare_func_arrival: (policy == SRPT_query) ? compare_func_size : (policy == NEW_1) ? compare_func_size : compare_func_arrival;

        for (double arrival_lambda=0.1; arrival_lambda<=5.8; arrival_lambda+=0.1) {

            ExponentialDistribution arrival_dist(arrival_lambda);
            ParetoDistribution phase_size_dist(1.5, 1);
            QueryGenerator query_generator(&arrival_dist, &phase_size_dist, n_cores);
                       
            int iteration = 10; 
            double mean_jobs = 0;
            for (int i=0; i<=(iteration-1); i++) {
                Simulation simulation(n_cores, policy, compare_func, query_generator);
                simulation.initialize();
                for (int j=0; j<10000;) {
                    simulation.run(&j);
                }
                mean_jobs += simulation.getMeanJobs();
            }
            mean_jobs = mean_jobs/iteration;

            data << arrival_lambda/(512*(1/435.72+1/49.03)/2) << ", " << mean_jobs << ", \n";

            // ExpExp(n_cores, arrival_lambda, phase_size_lambda - i, 100);
        }

        data.close();
    }

    return 1;
}

int testSpeedUp() {
    std::ofstream data;
    data.open ("../data/speed_up.csv");
    data << "cores, service_time, \n";
    std::map<int, double> cores_time;
    int n_cores = 512; 
    QueryGenerator query_generator;
    double time_one_core;
    for (int cores = 1; cores <= n_cores; cores++) {
        Query query {query_generator.generatePhases1(), 0};
        double time = query.testSpeedUp(cores);
        if (cores == 1) {
            time_one_core = time;
            data << cores << ", " << time <<  ", " << 1 << ", \n";
        } else {
            data << cores << ", " << time <<  ", " <<  time_one_core/time << ", \n";
        }
    }
    data.close();
    return 1;
}
int main() {
    return simulation();
}


