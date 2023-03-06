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
        if (phases.front().size < 0) {
            std::cout << time << "; " << n_cores << "; " << phases.front().size << std::endl; 
        }
        if (phases.front().size == 0) {
            finishPhase();
            return 1;
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
};
