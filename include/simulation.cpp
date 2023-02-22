#include <iostream>
#include "simulation.hpp"


Simulation::Simulation() {
    query_generator = new QueryGenerator(new ExponentialDistribution(2.0), new UniformDistribution(0.0, 1.0));
};

double Simulation::getTime() {
    return time;
}


double Simulation::getTimeC() {
    return time_c;
}


double Simulation::getTimeA() {
    return time_a;
}


int Simulation::getNJobs() {
    return n_jobs;
}


int Simulation::getNJobsProc() {
    return n_jobs_proc;
}


std::vector<Query *> Simulation::getQueue() {
    return queue;
}


std::vector<Query *> Simulation::getProcessor() {
    return processor;
}


void Simulation::initialize() {
    time_a = query_generator->getArrivalDist()->sample();
}

int Simulation::run() {
    
//    while (time < 100.0) {
    double time_n = std::min(time_a, time_c); // find time_next
    if (time_n == time_a) { // if time_next is time_arrival
        
        Simulation::procUpdate(time_n); // time_c update here
           
        std::cout << "arrival occurs; ";

        Query *query = Simulation::queryGenerate();

        // n_job++
        n_jobs++;
        //
        // if there is spare core, allocate it
        //      find out the new tc
        if (n_jobs_proc < multiprogramming) {
            Simulation::procAllocate(query);
        } else {
            // if not, put in the queue
            Simulation::queueAllocate(query);
        }

        // generate new time_a
        time_a = query_generator->getArrivalDist()->sample();

        time += time_n;

        Simulation::output();


        return 0;
        
    } else { // if time_n == time_c
        
        std::cout << "departure occurs; ";
        
        time_a -= time_n;

        Simulation::procUpdate(time_n); // update time_c
        Simulation::procRemove();

        // if there is anything on the queue, allocate it
        if (queue.size() > 0) {
            Simulation::procAllocate(Simulation::queueGet());
        }

        time += time_n;

        Simulation::output();

        return 1;
    }
    
}

Query *Simulation::queryGenerate() {
    return query_generator->nextP();
}

int Simulation::procAllocate(Query *query) {
    if (n_jobs_proc < multiprogramming) {
        processor.push_back(query);
        n_jobs_proc++;
        // calculate new time_c
        if ((query->size) < time_c) {
            time_c = query->size;
        }
        return 1;
    } else {
        return 0;
    }
}


void Simulation::procUpdate(double time) {
    if (processor.size() != 0) {
        time_c -= time;
        bool time_c_invalid = time_c == 0.0;
        if (time_c_invalid) {
            time_c = INFINITY;
        }
        for (Query * query : processor) {
            query->size -= time;
            if (time_c_invalid) {
                if ((query->size < time_c) && (query->size != 0)) {
                    time_c = query->size;
                }
            }
        }
    } else {
        time_c = INFINITY;
    }
}


void Simulation::procRemove() {
    for (int i=0; i<processor.size(); i++) {
        if (processor[i]->size == 0) {
            processor.erase(processor.begin()+i);
            n_jobs_proc--;
            n_jobs--;
        }
    }
}


int Simulation::queueAllocate(Query *query) {
    queue.push_back(query);
    return 1;
}


Query *Simulation::queueGet() {
    Query *query = queue.front();
    queue.erase(queue.begin());
    return query;
}


void Simulation::output() {
    std::cout << "n_jobs: " << n_jobs << ", time: " << time << ", time_a: " << time_a << ", time_c: " << time_c << std::endl;
}
