#include <iostream>
#include <mutex>
#include <set>
#include "simulation.hpp"


Simulation::Simulation(int cores, Policy policy, CompareFunc compare_func, QueryGenerator query_generator) : cores(cores), policy(policy), query_generator(query_generator), queue(compare_func), processor(compare_func) {

}

Simulation::~Simulation() {
    // delete this->queue;
}

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


std::multiset<Query *, Simulation::CompareFunc> Simulation::getQueue() {
    return queue;
}


std::multiset<Query *, Simulation::CompareFunc> Simulation::getProcessor() {
    return processor;
}


double Simulation::getJobsTime() {
    return jobs_time;
}


double Simulation::getMeanJobs() {
    return jobs_time/time;
}


void Simulation::initialize() {
    time_a = query_generator.getArrivalDist()->sample();
}

int Simulation::run() {

    double time_n = std::min(time_a, time_c); // find time_next
                                                  
    if (time_n == time_a) { // if time_next is time_arrival
        
        // std::cout << std::endl << "arrival occurs; ";
        
        Simulation::procUpdate(time_n); // time_c update here
        
        Query *query = query_generator.nextP(time_a);

        allocate(query); // allocate to processor/queue
       
        time_a = query_generator.getArrivalDist()->sample(); // generate new time_a

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

    // Simulation::output();
    
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
    
    if (policy == FCFS | policy == SJF) {
        return std::min(cores-used_cores, query->getNextPhase().multiprogramming);
    } 

    else if (policy == SRPT_query) {
        if (used_cores < cores) {
            return std::min(cores-used_cores, query->getNextPhase().multiprogramming);
        } else if (query->size < (*std::prev(processor.end()))->size) {
            return -std::min(cores - (used_cores - (*std::prev(processor.end()))->n_cores), query->getNextPhase().multiprogramming);
        } else {
            return 0;
        }
    }

    else if (policy == SRPT_phase) { // pseudo check
        if (used_cores < cores) return 1;
        else if (query->size < (*std::prev(processor.end()))->size) return 1;
    }
    
    return 0;
}


int Simulation::procAllocate(Query *query, int n_cores) {
    if (policy == SRPT_phase) {
        // for SRPT_phase, smaller size query will be allocated more cores -> only need to check new query to update time_c
        for (std::multiset<Query *, CompareFunc>::reverse_iterator query_p = processor.rbegin(); query_p != processor.rend();) {
            if (cores-used_cores >= query->getNextPhase().multiprogramming) {
                break;
            }
            Query *cur_query = *query_p;
            if (query->size < cur_query->size) {
                used_cores -= cur_query->n_cores;
                if (cores-used_cores > query->getNextPhase().multiprogramming) { // if this is it, there are extra cores
                    cur_query->setNCores(cores-used_cores-query->getNextPhase().multiprogramming);
                    used_cores += cur_query->n_cores;
                    break;
                } else {
                    processor.erase(std::next(query_p).base());
                    queueAllocate(cur_query);
                    continue;
                }
            } else {
                break;
            }
            ++query_p; // should never reach
        }
        query->setNCores(std::min(cores-used_cores, query->getNextPhase().multiprogramming));
        used_cores += query->n_cores;
        processor.insert(query);
        time_c = (query->getCurSize() < time_c) ? query->getCurSize() : time_c;
    }

    else {
        if (n_cores < 0) {
            Query *last_query = *std::prev(processor.end());
            used_cores -= last_query->n_cores;
            queueAllocate(last_query);
            bool time_c_invalid = last_query->getCurSize() == time_c;
            processor.erase(std::prev(processor.end()));
            if (time_c_invalid) {
                timeCUpdate();
            }
            n_cores = -n_cores;
        }
        query->setNCores(n_cores);
        processor.insert(query);
        used_cores += n_cores; 
        double tmp_time = query->getCurSize();
        time_c = (tmp_time < time_c) ? tmp_time : time_c;
    }

    return 1;
}


void Simulation::procUpdate(double time) {
    time_c = INFINITY;
    std::vector<Query *> pointers;

    for (std::multiset<Query *, Simulation::CompareFunc>::iterator query_p = processor.begin(); query_p != processor.end();) {

        if (policy == SRPT_phase) {
            Query *query = *query_p;
            query_p = processor.erase(query_p);
            used_cores -= query->n_cores;
            int phase_finish = query->updateSize(time);
            if (phase_finish & (query->phases.size() == 0)) {
                continue;
            }
            pointers.push_back(query); 
        }
        else {
            Query *query = *query_p;
            query_p = processor.erase(query_p);
            int phase_finish = query->updateSize(time);

            if (phase_finish == 1) {

                used_cores -= query->n_cores;
                int n_cores = check(query);
                if (n_cores == 0) {
                    if (query->phases.size() != 0) queueAllocate(query);
                } else {
                    query->setNCores(n_cores);
                    used_cores += n_cores;
                    double tmp_time = query->getCurSize();
                    time_c = (tmp_time < time_c) ? tmp_time : time_c;
                    pointers.push_back(query);
                }

            } else {
                pointers.push_back(query);
                
            }
        }
    }

    for (Query *query : pointers) {
        if (policy == SRPT_phase) {
            allocate(query);
        }
        else {
            if (query->getCurSize() < time_c) {
                time_c = query->getCurSize();
            }
            processor.insert(query);
        }
    }
}


void Simulation::timeCUpdate() {
    time_c = INFINITY;
    for (std::multiset<Query *, Simulation::CompareFunc>::iterator query_p = processor.begin(); query_p != processor.end(); ++query_p) {
        if ((*query_p)->getCurSize() < time_c) {
            time_c = (*query_p)->getCurSize();
        }
    }
}


int Simulation::queueAllocate(Query *query) {
    queue.insert(query);
    return 1;
}


int Simulation::queueGet() {
    for (auto query_p = queue.begin(); query_p != queue.end();) {
        if (used_cores < cores) {
            int n_cores = check(*query_p);
            if (n_cores != 0) {
                procAllocate(*query_p, n_cores);
                query_p = queue.erase(query_p);
            } else {
                ++query_p;
            }
        } else {
            break;
        }
    }
    return 1;
}
            

void Simulation::output() {
    
    std::cout << "time: " << time << ", time_a: " << time_a << ", time_c: " << time_c << ", used_cores: " << used_cores << std::endl;

    std::cout << "processor:" << std::endl;
    for (Query *query : processor) {
        query->printQuery();
    }
    std::cout << "queue:" << std::endl;
    for (Query *query : queue) {
        query->printQuery();
    }
    std::cout << getNJobs() << " " << jobs_time << std::endl;
}


void Simulation::printProcessor() {
    for (std::multiset<Query *, CompareFunc>::iterator query_p = processor.begin(); query_p != processor.end(); ++query_p) {
        (*query_p)->printQuery();
    }
}
