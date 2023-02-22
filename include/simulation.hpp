#include <cmath>
#include "generation.hpp"


class Simulation {
    private:
        int multiprogramming = 8;
        double time = 0.0;
        int n_jobs = 0;
        int n_jobs_proc = 0;
        double time_c = INFINITY; // time until next completion
        double time_a; // time until next arrival
        QueryGenerator *query_generator;
        std::vector<Query *> queue;
        std::vector<int> index_next_complete; // vector of indexs of processor that tracks the query that will firstly complete processing
        std::vector<Query *> processor;
    public:

        // class constructor 
        Simulation(); 

        double getTime();

        double getTimeC();

        double getTimeA();

        int getNJobs();

        int getNJobsProc();

        std::vector<Query *> getQueue();

        std::vector<Query *> getProcessor();

        // initialize the simulation: set time_a
        void initialize();

         // run the simulation
         // return 0 if an arrival occurs
         // return 1 if a departure occurs
        int run(); 

        // generate a query
        Query *queryGenerate();

        // if there is spare processor resources
        // insert Query * into processor
        // update n_jobs_proc
        // update time_c if necessary
        // return 1 if insertion is successful
        // return 0 if processor is full
        int procAllocate(Query *);

        // substract all queries' sizes in processor by a time
        // update time_c
        void procUpdate(double time);

        // remove all queries that finish processing
        // update n_jobs
        void procRemove();

        // insert Query * into queue
        // update time_c (next completion time)
        // return 1 if insertion is successful
        // return 0 if not  
        int queueAllocate(Query *);
        
        // get a query from queue
        // temporarily just FCFS (just get the first one)
        // remove the chosen query
        Query *queueGet();
        
        // produce necessary output data
        void output();
};
