#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include "simulation.hpp"


Simulation::Simulation(int cores, Policy policy, CompareFunc compare_func, QueryGenerator query_generator) : compare_func(compare_func), cores(cores), policy(policy), query_generator(query_generator), queue(compare_func), processor(compare_func) {

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

int Simulation::run(int *n) {

    double time_n = std::min(time_a, time_c); // find time_next
    
    //std::cout << "############################################" << std::endl;
                                                  
    if (time_n == time_a) { // if time_next is time_arrival
        
        //std::cout << "arrival occurs; ";
        
        Simulation::procUpdate(time_n, n); // time_c update here
        
        std::shared_ptr<Query> query = query_generator.nextP(time_a);

        allocate(query);
       
        time_a = query_generator.getArrivalDist()->sample(); // generate new time_a

        time += time_n;

    } else { // if time_n == time_c
        
        //std::cout << "phase finishes; ";

        time_a -= time_n;

        Simulation::procUpdate(time_n, n); // time_c update here
        
        time += time_n;

    }

    jobs_time += getNJobs() * time_n;

    //Simulation::output();
    
    //std::cout << std::endl;
    
    return 1;
    
}


// return 1 if allocated to processor, 0 otherwise
int Simulation::allocate(std::shared_ptr<Query> query) {
    if (policy == RR) {
        processor.insert(query);
        return 1;
    } else {
        if (used_cores < cores) {
            int cur_used_cores = query->allocate(cores - used_cores);
            if (cur_used_cores > 0) {
                used_cores += cur_used_cores;
                updateTimeC(query->getTimeC());
                processor.insert(query);
                return 1;
            }
        } 
        // allocate() is only called when arrival occurs, no preemption is allowed (no Block finishes)
        queueAllocate(query);
        return 0;
    }
}


void Simulation::procUpdate(double time, int *n) {
    
    if (policy == RR) {
        // update all Querys with cores allocated by time
        for (auto query_p = processor.begin(); query_p != processor.end();) {
            std::shared_ptr<Query> cur_query = *query_p;
            if (cur_query->cores != 0) {
                int finished = cur_query->update(time);
                cur_query->deallocate();
                if (finished) {
                    query_p = processor.erase(query_p);
                    *n += 1;
                    continue;
                }
            }
            ++query_p;
        }
        // reallocate all the cores

    } else {
        time_c = INFINITY;
        std::multiset<std::shared_ptr<Query>, CompareFunc> tmp_processor(compare_func);

        if (time == time_a) {
            for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
                std::shared_ptr<Query> cur_query_p = *query_p;
                cur_query_p->update(time);
                tmp_processor.insert(cur_query_p);
                updateTimeC(cur_query_p->getTimeC());
            }
            processor.swap(tmp_processor);
        } else {
            for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
                std::shared_ptr<Query> cur_query = *query_p;
                used_cores -= cur_query->cores;
                int finished = cur_query->update(time);
                if (finished == 0) {
                    used_cores += cur_query->cores;
                    updateTimeC(cur_query->getTimeC());
                    tmp_processor.insert(cur_query);
                } else if (finished == -1) {
                    queueAllocate(cur_query);
                } else if (finished == 1) {
                    *n += 1;
                }
            }
            processor.swap(tmp_processor);
            if (policy == NEW_1) {
                queueToProc();
                for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
                    if (used_cores < cores) {
                        std::shared_ptr<Query> cur_query = *query_p;
                        int cur_cores = cur_query->allocate(cores - used_cores);
                        if (cur_cores > 0) {
                            used_cores += cur_cores;
                            updateTimeC(cur_query->getTimeC());
                        }
                    } else {
                        break;
                    }
                }
            } else {
                for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
                    while (used_cores < cores && queue.size() > 0 && compare_func(*queue.begin(), *query_p)) {
                        std::shared_ptr<Query> cur_query = *queue.begin();
                        int cur_used_cores = cur_query->allocate(cores - used_cores);
                        if (cur_used_cores > 0) {
                            used_cores += cur_used_cores;
                            updateTimeC(cur_query->getTimeC());
                            processor.insert(cur_query);
                            queue.erase(queue.begin());
                        }
                    }
                    used_cores += (*query_p)->allocate(cores - used_cores);
                    updateTimeC((*query_p)->getTimeC());
                }
                if (used_cores < cores && queue.size() > 0) {
                    queueToProc();
                }
            }
        }
    }
}


int Simulation::queueToProc() {
    for (auto query_p = queue.begin(); query_p != queue.end();) {
        if (used_cores == cores) {
            break;
        }
        int alloc_cores;
        if (policy == NEW_1) {
            alloc_cores = 1;
        } else {
            alloc_cores = cores - used_cores;
        }
        int cur_used_cores = (*query_p)->allocate(alloc_cores);
        if (cur_used_cores > 0) {
            used_cores += cur_used_cores;
            updateTimeC((*query_p)->getTimeC());
            processor.insert(*query_p);
            query_p = queue.erase(query_p);
        } else {
            ++query_p;
        }
    }
    return 1;
}

void Simulation::updateTimeC(double time_query_c) {
    time_c = (time_query_c < time_c) ? time_query_c : time_c;
}


int Simulation::queueAllocate(std::shared_ptr<Query> query) {
    queue.insert(query);
    return 1;
}


void Simulation::output() {
    
    std::cout << "time: " << time << ", time_a: " << time_a << ", time_c: " << time_c << ", used_cores: " << used_cores << std::endl;

    std::cout << "processor:" << std::endl;
    int idx = 0;
    for (std::shared_ptr<Query> query : processor) {
        std::cout << "query_" << idx << std::endl;
        query->printQuery();
        idx++;
    }
    std::cout << "queue_size:" << queue.size() << std::endl;
    //idx = 0;
    //for (std::shared_ptr<Query> query : queue) {
        //std::cout << "query_" << idx << std::endl;
        //query->printQuery(1);
        //idx++;
    //}
    std::cout << getNJobs() << " " << jobs_time << std::endl;
}


void Simulation::printProcessor() {
    for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
        (*query_p)->printQuery();
    }
}

