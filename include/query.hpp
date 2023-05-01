#ifndef QUERY_H
#define QUERY_H

#include "phase.hpp"
#include <vector>
#include <iostream>

struct Query {
    std::vector <Phase> phases;
    double arrival; // arrival time
    double size; // remaining processing time
    double memory = 0; // amount of memory required
    int n_cores = 0; // number of cores it is running on

    Query(std::vector<Phase> phases_, double arrival_, double size_, double memory_, int n_cores_) : phases(phases_), arrival(arrival_), size(size_), memory(memory_), n_cores(n_cores_) {}
    
    Phase getNextPhase() {
        return phases.front();
    }

    double getCurSize() {
        return phases.front().size/n_cores;
    }

    void finishPhase() {
        phases.erase(phases.begin());
    }

    int updateSize(double time) {
        time = time * n_cores;
        phases.front().size -= time;
        size -= time;
        
        if (std::abs(phases.front().size) <= 1e-7f) {
            finishPhase();
            return 1;
        }
        if (phases.front().size < 0) {
            printQuery();
            throw "invalid phase size.";
        } 
            return 0;
    }

    int setNCores(int n) {
        if (n <= getNextPhase().multiprogramming) {
            n_cores = n;
            return 1;
        }
        return 0;
    }

    void printQuery() {
        std::cout << "n_cores: " << n_cores << ", size: " << size << ", phases: ";
        for (Phase phase : phases) {
            std::cout << "{" << phase.multiprogramming << ", " << phase.size << "}, ";
        }
        std::cout << std::endl;
    }
};

#endif
