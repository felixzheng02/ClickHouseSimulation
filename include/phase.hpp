#ifndef PHASE_H
#define PHASE_H

#include <iostream>

struct Phase {
    int multiprogramming = 1; // 1: inelastic; more than 1: elastic
    int cores = 0;
    double size; // remaining processing time given one CPU core
   
    // return decremented size
    double update(double time) {
        double size_dec = time * cores;
        size -= size_dec;
        return size_dec;
    }

    double getTimeC() {
        return size/cores;
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
