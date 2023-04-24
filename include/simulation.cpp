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

        allocate(query);
       
        time_a = query_generator.getArrivalDist()->sample(); // generate new time_a

        time += time_n;

    } else { // if time_n == time_c
        
        //std::cout << std::endl << "phase finishes; ";

        time_a -= time_n;

        Simulation::procUpdate(time_n); // time_c update here
        
        time += time_n;

    }

    jobs_time += getNJobs() * time_n;

    //Simulation::output();
    
    return 1;
    
}


// return 1 if allocated to processor, 0 otherwise
int Simulation::allocate(std::shared_ptr<Query> query) {
    if (used_cores < cores) {
        used_cores += query->allocate(cores - used_cores);
        updateTimeC(query->getTimeC());
        processor.insert(query);
        return 1;
    } else {
        // allocate() is only called when arrival occurs, no preemption is allowed (no Block finishes)
        queueAllocate(query);
        return 0;
    }
}


void Simulation::procUpdate(double time) {

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
            int finished = cur_query->update(time);
            if (finished) {
                queueAllocate(cur_query);
                processor.erase(query_p);
                continue;
            }
        }
        for (auto query_p = processor.begin(); query_p != processor.end(); ++query_p) {
            if (used_cores < cores) {
                while (compare_func(*queue.begin(), *query_p)) {
                    std::shared_ptr<Query> cur_query = *queue.begin();
                    used_cores += cur_query->allocate(cores - used_cores);
                    processor.insert(cur_query);
                    updateTimeC(cur_query->getTimeC());
                    queue.erase(queue.begin());
                }
                (*query_p)->allocate(cores - used_cores);
            } else {
                break;
            }
        }
    }
}



void Simulation::updateTimeC(double time_query_c) {
    time_c = (time_query_c < time_c) ? time_query_c : time_c;
}


int Simulation::queueAllocate(std::shared_ptr<Query> query) {
    used_cores += query->cores;
    queue.insert(query);
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
