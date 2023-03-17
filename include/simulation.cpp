#include <iostream>
#include "simulation.hpp"


Simulation::Simulation(int cores, Policy policy,  QueryGenerator *query_generator) {
    this->cores = cores;
    this->policy = policy;
    this->query_generator = query_generator;
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
    return queue.size() + processor.size();
}


std::list<Query *> Simulation::getQueue() {
    return queue;
}


std::vector<Query *> Simulation::getProcessor() {
    return processor;
}


double Simulation::getJobsTime() {
    return jobs_time;
}


double Simulation::getMeanJobs() {
    return jobs_time/time;
}


void Simulation::initialize() {
    time_a = query_generator->getArrivalDist()->sample();
}

int Simulation::run() {

    double time_n = std::min(time_a, time_c); // find time_next
                                                  
    if (time_n == time_a) { // if time_next is time_arrival
        
        // std::cout << std::endl << "arrival occurs; ";
        
        Simulation::procUpdate(time_n); // time_c update here

        Query *query = query_generator->nextP(time_a);

        allocate(query); // allocate to processor/queue
                         
        time_a = query_generator->getArrivalDist()->sample(); // generate new time_a

        time += time_n;

    } else { // if time_n == time_c
        
        // std::cout << std::endl << "phase finishes; ";
        
        time_a -= time_n;

        Simulation::procUpdate(time_n); // time_c update here
        
        if (queue.size() > 0) {
            queueGet();
        }
        
        time += time_n;

    }

    jobs_time += getNJobs() * time_n;

    if (getNJobs() < 0) {
        throw "Invalid number of jobs.";
    }

    Simulation::output();

    return 1;

}


int Simulation::allocate(Query *query) {
    int n_cores = check(query);
    if (n_cores != 0) {
        procAllocate(query, n_cores);
        return 1;
    } else {
        queueAllocate(query);
        return 0;
    }
}


int Simulation::check(Query *query) {
    if (query->phases.size() == 0) return 0;
    if (query->getNextPhase().multiprogramming == 1) {
        if (used_cores < cores) {
            return 1;
        }
        return 0;
    } else {
        if (used_cores < cores) {
            return std::min(cores-used_cores, query->getNextPhase().multiprogramming);
        }
        return 0;
    }
}


int Simulation::procAllocate(Query *query, int n_cores) {
    query->setNCores(n_cores);
    processor.push_back(query);
    used_cores += n_cores; 
    double tmp_time = query->getCurSize();
    time_c = (tmp_time < time_c) ? tmp_time : time_c;
    return 1;
}


void Simulation::procUpdate(double time) {
    time_c = INFINITY;
    for (int i=0; i<processor.size(); i++) {
        int phase_finish = processor[i]->updateSize(time);
        if (phase_finish == 1) {
            used_cores -= processor[i]->n_cores;
            Query *query = processor[i];
            if (query->phases.size() == 0) {
                processor.erase(processor.begin()+i);
                i--;
                continue;
            }
            int n_cores = check(query);
            if (n_cores == 0) {
                processor.erase(processor.begin()+i);
                queueAllocate(query);
                i--;
            } else {
                query->setNCores(n_cores);
                used_cores += n_cores;
                if (query->getCurSize() < time_c) {
                    time_c = query->getCurSize();
                }
            }
        } else {
            if (processor[i]->getCurSize() < time_c) {
                time_c = processor[i]->getCurSize();
            }
        }
    }
}    


int Simulation::queueAllocate(Query *query) {

    if (policy == FCFS) {
        for (std::list<Query *>::iterator query_p = queue.begin(); query_p != queue.end(); ++query_p) {
            if (query->arrival < (*query_p)->arrival) {
                queue.insert(query_p, query);
                return 1;
            }
        }
        queue.push_back(query);
    }

    else if (policy == SJF) {
        for (std::list<Query *>::iterator query_p = queue.begin(); query_p != queue.end(); ++query_p) {
            if (query->size < (*query_p)->size) {
                queue.insert(query_p, query);
                return 1;
            }
        }
        queue.push_back(query);
    }

    return 1;
}


int Simulation::queueGet() {
    for (std::list<Query *>::iterator query_p = queue.begin(); query_p != queue.end(); ++query_p) {
        if (used_cores >= cores) {
            return 1;
        }
        int n_cores = check(*query_p);
        if (n_cores != 0) {
            procAllocate(*query_p, n_cores);
            queue.erase(query_p);
        }
    }
    return 1;
}
            

void Simulation::output() {
    /*
    std::cout << "time: " << time << ", time_a: " << time_a << ", time_c: " << time_c << ", used_cores: " << used_cores << ", queue_size: " << queue.size() << std::endl;

    std::cout << "processor:" << std::endl;
    for (Query *query : processor) {
        query->printQuery();
    }
    std::cout << "queue:" << std::endl;
    for (Query *query : queue) {
        query->printQuery();
    }
    std::cout << getNJobs() << " " << jobs_time << std::endl;
    */
}
