#ifndef PHASE_H
#define PHASE_H

#include <iostream>

struct Phase {
    int multiprogramming = 1; // 1: inelastic; more than 1: elastic
    int cores = 0;
    double size; // remaining processing time given one CPU core
   
    // return 1 if finished running
    int update(double time, double *query_size, int *query_cores, double *query_time_c) {
        double size_dec = time * cores;
        size -= size_dec;
        *query_size -= size_dec;
        if (std::abs(size) <= 1e-7f) {
            *query_cores -= cores;
            cores = 0;
            return 1;
        }
        if (size/cores < *query_time_c) {
            *query_time_c = size/cores;
        }
        return 0;
    }

    int updateRR(double time, double *query_size) {
        size -= time;
        *query_size -= time;
        if (std::abs(size) <= 1e-7f) {
            return 1;
        }
        return 0;
    }

    double getTimeC() {
        return size/cores;
    }

    int allocate(int n_cores, int *query_cores, double *query_time_c) {
        cores += n_cores;
        *query_cores += n_cores;
        if (size/cores < *query_time_c) {
            *query_time_c = size/cores;
        }
        return 1;
    }

    int printPhase() {
        std::cout << "multiprogramming: " << multiprogramming << ", cores: " << cores << ", size: " << size;
        return 1;
    }
};

#endif
