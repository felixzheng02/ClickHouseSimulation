#ifndef PHASE_H
#define PHASE_H

#include <iostream>

struct Phase {
    int multiprogramming = 1; // 1: inelastic; more than 1: elastic
    int cores = 0;
    double size; // remaining processing time given one CPU core
   
    // return 1 if current Phase is finished
    int update(double time) {
        size -= time * cores;
        if (std::abs(size) <= 1e-7f) {
            return 1;
        }
        return 0;
    }

    int allocate(int n_cores) {
        cores = n_cores;
        return 1;
    }

    int printPhase() {
        std::cout << "multiprogramming: " << multiprogramming << ", cores: " << cores << ", size: " << size;
        return 1;
    }
};

#endif
