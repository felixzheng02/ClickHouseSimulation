#include "phase.hpp"
#include <vector>
#include <iostream>

struct Query {
    std::vector <Phase> phases;
    double arrival; // arrival time
    double size; // remaining processing time
    double memory = 0; // amount of memory required
    int n_cores = 0; // number of cores it is running on
    
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
        std::cout << "n_cores: " << n_cores << ", phases: ";
        for (Phase phase : phases) {
            std::cout << "{" << phase.multiprogramming << ", " << phase.size << "}, ";
        }
        std::cout << std::endl;
    }
};
