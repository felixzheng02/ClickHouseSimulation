#ifndef PHASE_H
#define PHASE_H

#include <iostream>
#include <cmath>
#include "generation.hpp"

struct Phase {
    int multiprogramming = 1; // 1: inelastic; more than 1: elastic
    int cores = 0;
    long double size; // remaining processing time given one CPU core
  
    Phase(int multiprogramming, double size) : multiprogramming(multiprogramming), size(size) {
    }
    // return 1 if Phase finishes
    int update(double time, double *query_size, int *query_cores, double *query_time_c) {
        if (std::isnan(size)) {
            return 0;
        }
        double size_dec = time * cores;
        size -= size_dec;
        if (std::isnan(size)) {
            return 0;
        }
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


    double getTimeC() {
        return size/cores;
    }

    // return used cores
    int allocate(int n_cores, int *query_cores, double *query_time_c) {
        int before_cores = cores;
        cores = std::min(cores + n_cores, multiprogramming);
        if (cores - before_cores > 0) {
            *query_cores += (cores - before_cores);
            if (size/cores < *query_time_c) {
                *query_time_c = size/cores;
            }
            return cores - before_cores;
        }
        return 0;
    }

    int deallocate() {
        cores = 0;
        return 1;
    }

    int printPhase() {
        std::cout << "multiprogramming: " << multiprogramming << ", cores: " << cores << ", size: " << size;
        return 1;
    }
};

#endif
