#include "../include/simulation.hpp"
#include <cassert>
#include <iostream>


int main() {

    Simulation *simulation = new Simulation();

    simulation->initialize();

    double time = simulation->getTime();
    double time_c = simulation->getTimeC();
    double time_a = simulation->getTimeA();
    int n_jobs = simulation->getNJobs();
    int n_jobs_proc = simulation->getNJobsProc();
    std::vector<Query *> queue = simulation->getQueue(); 
    std::vector<Query *> processor = simulation->getProcessor(); 

    for (int i=0; i<1000; i++) {
        int result = simulation->run(); // 0 if arrival, 1 if departure
        
        double time_new = simulation->getTime();
        double time_c_new  = simulation->getTimeC();
        double time_a_new  = simulation->getTimeA();
        int n_jobs_new  = simulation->getNJobs();
        int n_jobs_proc_new  = simulation->getNJobsProc();
        std::vector<Query *> queue_new  = simulation->getQueue(); 
        std::vector<Query *> processor_new  = simulation->getProcessor(); 
   
        if (result == 0) {
            assert (time_new == time + time_a);
            if (n_jobs_proc_new == n_jobs_proc + 1) {
                double new_query_size = processor_new.back()->size;
                assert (time_c_new == time_c - time_a || time_c_new == new_query_size);
                assert (processor_new.size() == processor.size() + 1);
            } else {
                assert (queue_new.size() == queue.size() + 1);
            }
            assert (n_jobs_new == n_jobs + 1);
        } else {
            assert (time_new == time + time_c);
            assert (time_a_new == time_a - time_c);
            assert (n_jobs_new == n_jobs - 1);
            if (queue_new.size() == queue.size()) {
                assert (n_jobs_proc_new == n_jobs_proc - 1);
            } else {
                assert (n_jobs_proc_new == n_jobs_proc);
            }
        }
        
        time = time_new;
        time_c = time_c_new;
        time_a = time_a_new;
        n_jobs = n_jobs_new;
        n_jobs_proc = n_jobs_proc_new;
        queue = queue_new;
        processor = processor_new;

    }

    return 1;
}
