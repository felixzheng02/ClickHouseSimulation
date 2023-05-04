#ifndef SIMULATION_H
#define SIMULATION_H

#include <cmath>
#include <memory>
#include <vector>
#include <set>
#include "generation.hpp"
#include "query_generation.hpp"


/*
FCFS
SJF
SRPT_query
SRPT
NEW_1
    Attempt to allocate one query one core.
    Put extra queries on queue.
    Allocate extra cores based on SRPT fashion.
*/

enum Policy {
    RR,
    SRPT_query,
    NEW_1
};

std::string getText(Policy policy);


class Simulation {
    protected:
        typedef bool (*CompareFunc)(std::shared_ptr<Query> query_1, std::shared_ptr<Query> query_2);
        CompareFunc compare_func;
        int cores = 8; // total cores available
        int used_cores = 0; // number of cores occupied
        double time = 0.0; // global time
        double time_a; // time until next arrival
        double time_c = INFINITY; // time until next phase completion
        QueryGenerator query_generator;
        std::multiset<std::shared_ptr<Query>, CompareFunc> queue;
        Policy policy;
        std::multiset<std::shared_ptr<Query>, CompareFunc> processor;
        double jobs_time = 0; // keep track of average number of jobs in the system * global time
        
        // attempt to allocate the query to the processor
        // if cannot do that, allocate to the queue
        int allocate(std::shared_ptr<Query> query);

        int deallocate(std::shared_ptr<Query> query);

        // check if a query can be allocated in processor
        // return number of cores that should be allocated
        // return 0 if cannot be allocated
        int check(std::shared_ptr<Query> query);

        // insert Query * into processor
        // update used_cores
        // update time_c
        int procAllocate(std::shared_ptr<Query> query);

        // update each query's current phase's size
        // if current phase finishes, start next phase
        // update time_c
        void procUpdate(double time, int* n);
       
        void updateTimeC(double time_query_c);

        // insert Query * into queue
        int queueAllocate(std::shared_ptr<Query> query);
       
        // allocate queries in the queue to the processor
        // based on scheduling policy (currently FCFS)
        // until processor's cores are all occupied
        int queueGet();

        int queueToProc();


    public:

        Simulation(int cores, Policy policy, CompareFunc compare, QueryGenerator query_generator); 

        double getTime();

        double getTimeA();

        double getTimeC();

        int getNJobs();

        std::multiset<std::shared_ptr<Query>, CompareFunc> getQueue();

        std::multiset<std::shared_ptr<Query>, CompareFunc> getProcessor();

        double getJobsTime();

        double getMeanJobs();
        
        // initialize the simulation
        // set time_a
        void initialize();

         // run the simulation till next time point
        int run(int *n); 
        
        
        // produce information
        void output();

        void printProcessor();
};

#endif
