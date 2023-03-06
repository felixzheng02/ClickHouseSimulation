#include <cmath>
#include "generation.hpp"


class Simulation {
    private:
        int cores = 8;
        int used_cores = 0;
        double time = 0.0;
        double time_a; // time until next arrival
        double time_c = INFINITY;
        QueryGenerator *query_generator;
        std::vector<Query *> queue;
        std::vector<int> index_next_complete; // vector of indexs of processor that tracks the query that will firstly complete processing
        std::vector<Query *> processor;
        //std::vector<Query *> processor_elastic;
    public:

        // class constructor 
        Simulation(); 

        double getTime();

        double getTimeC();

        double getTimeA();

        int getNJobs();

        int getNInelasticJobs();

        int getNElasticJobs();

        int getNCoresElastic();

        std::vector<Query *> getQueue();

        std::vector<Query *> getProcessor();
        
        //std::vector<Query *> getProcessorElastic();

        // initialize the simulation: set time_a
        void initialize();

         // run the simulation
         // return 0 if an arrival occurs
         // return 1 if a departure occurs
        int run(); 
        
        // allocate the query to the processor or the queue
        // update n_inelastic_jobs/n_elastic_jobs/n_queue_jobs
        // update time_c
        int allocate(Query *query);

        // check if a query can be allocated in processor
        // return number of cores that should be allocated
        // return 0 if cannot be allocated
        int check(Query *query);

        // insert Query * into processor
        // update time_c
        // return 1 if insertion is successful
        // return 0 if processor is full
        int procAllocate(Query *query, int n_cores);

        // update each query's current phase's size
        // if current phase finishes, start next phase
        // update time_c
        void procUpdate(double time);

        // remove all queries that finish processing
        // update n_inelastic_jobs/n_elstic_jobs
        // update time_c
        void procRemove();

        // insert Query * into queue
        // update time_c (next completion time)
        // return 1 if insertion is successful
        // return 0 if not  
        int queueAllocate(Query *query);
        
        // get a query from queue
        // temporarily just FCFS (just get the first one)
        // remove the chosen query
        Query *queueGet();
        
        // produce necessary output data
        void output();
};
