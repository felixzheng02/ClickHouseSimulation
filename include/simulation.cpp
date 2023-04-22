#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include "simulation.hpp"


Simulation::Simulation(int cores, Policy policy, CompareFunc compare_func, QueryGenerator query_generator) : compare_func(compare_func), cores(cores), policy(policy), query_generator(query_generator), queue(compare_func), processor(compare_func) {

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


std::multiset<std::shared_ptr<Query>, Simulation::CompareFunc> Simulation::getQueue() {
    return queue;
}


std::multiset<std::shared_ptr<Query>, Simulation::CompareFunc> Simulation::getProcessor() {
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
        
        //std::cout << std::endl << "arrival occurs; ";
        
        Simulation::procUpdate(time_n); // time_c update here
        
        std::shared_ptr<Query> query = query_generator.nextP(time_a);

        allocate(query); // allocate to processor/queue
       
        time_a = query_generator.getArrivalDist()->sample(); // generate new time_a

        time += time_n;

    } else { // if time_n == time_c
        
        //std::cout << std::endl << "phase finishes; ";

        time_a -= time_n;

        Simulation::procUpdate(time_n); // time_c update here
        
        if (queue.size() > 0) {
            queueGet();
        }
        
        time += time_n;

    }

    jobs_time += getNJobs() * time_n;

    //Simulation::output();
    
    return 1;
    
}


int Simulation::allocate(std::shared_ptr<Query> query) {
    int n_cores = check(query);
    if (n_cores != 0) {
        procAllocate(query, n_cores);
        return 1;
    } else {
        queueAllocate(query);
        return 0;
    }
}


int Simulation::check(std::shared_ptr<Query> query) {
    if (query->phases.size() == 0) return 0;
    
    else if (policy == SRPT_query) {
        if (used_cores < cores) {
            return std::min(cores-used_cores, query->getNextPhase().multiprogramming);
        } else if (query->size < (*std::prev(processor.end()))->size) {
            return -std::min(cores - (used_cores - (*std::prev(processor.end()))->n_cores), query->getNextPhase().multiprogramming);
        } else {
            return 0;
        }
    }

    else if (policy == NEW_1) {
        if (used_cores < cores) return 1;
        else if ((*processor.begin())->n_cores > 1) return 1;
        else if (query->size < (*std::prev(processor.end()))->size) return 1;
    }

    return 0;
}


int Simulation::procAllocate(std::shared_ptr<Query> query, int n_cores) {
    
    if (policy == NEW_1) {
        std::multiset<std::shared_ptr<Query>, CompareFunc> tmp_processor(compare_func);
        tmp_processor.insert(query);
        used_cores = 1;
        for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
            std::shared_ptr<Query> cur_query = *query_p;
            if (used_cores < cores) {
                cur_query->setNCores(1);
                tmp_processor.insert(cur_query);
                used_cores++;
            } else {
                cur_query->setNCores(0);
                queueAllocate(cur_query);
                break;
            }
        }
        processor.swap(tmp_processor);
        for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
            std::shared_ptr<Query> cur_query = *query_p;
            if (used_cores < cores) {
                cur_query->setNCores(std::min(cores-used_cores+1, cur_query->getNextPhase().multiprogramming));
                used_cores += cur_query->n_cores-1;
                time_c = (cur_query->getCurSize() < time_c) ? cur_query->getCurSize() : time_c; 
            } else {
                time_c = (cur_query->getCurSize() < time_c) ? cur_query->getCurSize() : time_c; 
                continue;
            }
        }
    }

    else {
        if (n_cores < 0) {
            std::shared_ptr<Query> last_query = *std::prev(processor.end());
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
    std::vector<std::shared_ptr<Query>> pointers;

    for (auto query_p = processor.begin(); query_p != processor.end();) {

        if (policy == NEW_1) {
            std::shared_ptr<Query> query = *query_p;
            query_p = processor.erase(query_p);
            used_cores -= query->n_cores;
            int phase_finish = query->updateSize(time);
            if (phase_finish & (query->phases.size() == 0)) {
                continue;
            }
            pointers.push_back(query); 
        }
        else {
            std::shared_ptr<Query> query = *query_p;
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

    for (std::shared_ptr<Query> query : pointers) {
        if (policy == SRPT | policy == NEW_1) {
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
    for (std::multiset<std::shared_ptr<Query>, Simulation::CompareFunc>::iterator query_p = processor.begin(); query_p != processor.end(); ++query_p) {
        if ((*query_p)->getCurSize() < time_c) {
            time_c = (*query_p)->getCurSize();
        }
    }
}


int Simulation::queueAllocate(std::shared_ptr<Query> query) {
    query->setNCores(0);
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
    for (std::shared_ptr<Query> query : processor) {
        query->printQuery();
    }
    std::cout << "queue:" << std::endl;
    for (std::shared_ptr<Query> query : queue) {
        query->printQuery();
    }
    std::cout << getNJobs() << " " << jobs_time << std::endl;
}


void Simulation::printProcessor() {
    for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
        (*query_p)->printQuery();
    }
}
