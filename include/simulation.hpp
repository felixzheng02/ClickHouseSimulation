#include <cmath>
#include <vector>
#include "generation.hpp"


class Simulation {
    private:

        int cores = 8; // total cores available
        int used_cores = 0; // number of cores occupied
        double time = 0.0; // global time
        double time_a; // time until next arrival
        double time_c = INFINITY; // time until next phase completion
        QueryGenerator *query_generator;
        std::vector<Query *> queue;
        std::vector<Query *> processor;

    public:

        Simulation(int cores, QueryGenerator *query_generator); 

        double getTime();

        double getTimeA();

        double getTimeC();

        int getNJobs();

        std::vector<Query *> getQueue();

        std::vector<Query *> getProcessor();
        
        // initialize the simulation
        // set time_a
        void initialize();

         // run the simulation till next time point
         // return 0 if an arrival occurs
         // return 1 if a departure occurs
        int run(); 
        
        // attempt to allocate the query to the processor
        // if cannot do that, allocate to the queue
        int allocate(Query *query);

        // check if a query can be allocated in processor
        // return number of cores that should be allocated
        // return 0 if cannot be allocated
        int check(Query *query);

        // insert Query * into processor
        // update used_cores
        // update time_c
        int procAllocate(Query *query, int n_cores);

        // update each query's current phase's size
        // if current phase finishes, start next phase
        // update time_c
        void procUpdate(double time);

        // insert Query * into queue
        int queueAllocate(Query *query);
       
        // allocate queries in the queue to the processor
        // based on scheduling policy (currently FCFS)
        // until processor's cores are all occupied
        int queueGet();
        
        // produce information
        void output();
};
