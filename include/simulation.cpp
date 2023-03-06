#include <iostream>
#include "simulation.hpp"


Simulation::Simulation() {
    query_generator = new QueryGenerator(new ExponentialDistribution(1.0), new UniformDistribution(0.0, 5.0));
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


std::vector<Query *> Simulation::getQueue() {
    return queue;
}


std::vector<Query *> Simulation::getProcessor() {
    return processor;
}


//std::vector<Query *> Simulation::getProcessorElastic() {
//    return processor_elastic;
//}


void Simulation::initialize() {
    time_a = query_generator->getArrivalDist()->sample();
}

int Simulation::run() {
    
    double time_n = std::min(time_a, time_c); // find time_next
                                                  
    if (time_n == time_a) { // if time_next is time_arrival
        
        std::cout << "arrival occurs; ";
        
        Simulation::procUpdate(time_n); // time_c update here
           
        Query *query = query_generator->nextP(time_a);

        allocate(query); // allocate to processor/queue
                         
        // generate new time_a
        time_a = query_generator->getArrivalDist()->sample();

        time += time_n;

        Simulation::output();

        return 0;
        
    } else { // if time_n == time_c
        
        std::cout << "phase finishes; ";
        
        time_a -= time_n;

        Simulation::procUpdate(time_n); // update time_c
        
        // when to schedule a query, how?
        if (queue.size() > 0) {
            Simulation::allocate(Simulation::queueGet());
        }
        
        time += time_n;

        Simulation::output();
        
        return 1;
    }
    
}


int Simulation::allocate(Query *query) {
    int n_cores = check(query);
    if (n_cores != 0) {
        procAllocate(query, n_cores);
        return 1;
    } else { // if n_inelastic_jobs == cores, allocate on queue
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
    std::vector<Query *> tmp;
    for (int i=0; i<processor.size(); i++) {
        int phase_finish = processor[i]->updateSize(time);
        if (phase_finish == 1) {
            used_cores -= processor[i]->n_cores;
            Query *query = processor[i];
            if (query->phases.size() == 0) {
                processor.erase(processor.begin()+i);
                break;
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
            while (used_cores < cores && queue.size()>0) {
                Query *query = queueGet();
                int n_cores = check(query);
                if (n_cores != 0) {
                    query->setNCores(n_cores);
                    tmp.push_back(query);
                    used_cores += n_cores;
                    if (processor[i]->getCurSize() < time_c) {
                        time_c = processor[i]->getCurSize();
                    }
                } else {
                    queueAllocate(query);
                }
            }
        } else {
            if (processor[i]->getCurSize() < time_c) {
                time_c = processor[i]->getCurSize();
            }
        }
    }
    for (Query *query : tmp) {
        processor.push_back(query);
    }

}    


int Simulation::queueAllocate(Query *query) {
    queue.push_back(query);
    return 1;
}


Query *Simulation::queueGet() {
    for (int i=0; i<queue.size(); i++) {
        Query *query = queue[i];
        if (check(query) != 0) {
            queue.erase(queue.begin()+i);
            return query;
        }
    }
    return NULL;
}
            

void Simulation::output() {
    std::cout << "time: " << time << ", time_a: " << time_a << ", time_c: " << time_c << ", used_cores: " << used_cores << ", queue_size: " << queue.size() << std::endl;
}
