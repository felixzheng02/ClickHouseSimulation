#ifndef BLOCK_H 
#define BLOCK_H 

#include <type_traits>
#include <vector>
#include <random>
#include "phase.hpp"

struct Block {
    std::vector<Phase> phases; // never change phases directly, use running_phases and waiting_phases
    std::vector<int> running_phases;
    std::vector<int> waiting_phases;

    Block(std::vector<Phase> phases) : phases(phases) {
        for (int idx = 0; idx < phases.size(); idx++) {
            waiting_phases.push_back(idx);
        }
    }
    
    // return how many cores used
    int allocate(int n_cores, int *query_cores, double *query_time_c) {
        int left_cores = n_cores;
        std::vector<int> allocated_phases;
        while (left_cores > 0 && (waiting_phases.size() != 0 || allocated_phases.size() != 0)) {
            left_cores = allocatePhase(left_cores, query_cores, query_time_c, &waiting_phases, &allocated_phases, &running_phases);
            left_cores = allocateAlloc(left_cores, query_cores, query_time_c,  &allocated_phases, &running_phases);
        }
        running_phases.insert(running_phases.end(), allocated_phases.begin(), allocated_phases.end());
        return n_cores - left_cores;
    }
    

    int allocatePhase(int n_cores, int *query_cores, double *query_time_c, std::vector<int> *low, std::vector<int> *mid, std::vector<int> *up) {
        
        for (auto idx_p = low->begin(); idx_p != low->end();) {
            if (n_cores == 0) return n_cores;
            Phase *phase = &phases[(*low)[*idx_p]];

            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr_cores(1, std::min(n_cores, phase->multiprogramming - phase->cores));
            int cores = distr_cores(gen);

            phase->allocate(cores, query_cores, query_time_c); 
            n_cores -= cores;
            if (phase->cores == phase->multiprogramming) {
                up->push_back((*low)[*idx_p]);
            } else {
                mid->push_back((*low)[*idx_p]);
            }
            idx_p = low->erase(idx_p);
        }
        return n_cores;
    }

    int allocateAlloc(int n_cores, int *query_cores, double *query_time_c, std::vector<int> *low, std::vector<int> *up) {
        
        std::vector<int> tmp_idx;
        for (auto idx_p = low->begin(); idx_p != low->end();) {
            if (n_cores == 0) return n_cores;
            Phase *phase = &phases[(*low)[*idx_p]];

            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr_cores(1, std::min(n_cores, phase->multiprogramming - phase->cores));
            int cores = distr_cores(gen);

            phase->allocate(cores, query_cores, query_time_c); 
            n_cores -= cores;
            if (phase->cores == phase->multiprogramming) {
                up->push_back((*low)[*idx_p]);
            } else {
                tmp_idx.push_back((*low)[*idx_p]);
            }
            idx_p = low->erase(idx_p);
        }
        low->insert(low->end(), tmp_idx.begin(), tmp_idx.end());
        return n_cores;
    }

    // return 1 if all current running Phases are finished (can be preempted)
    int update(double time, double *query_size, int *query_cores, double *query_time_c) {
        for (int idx = 0; idx < running_phases.size();) {
            Phase *cur_phase = &phases[running_phases[idx]];
            int finished = cur_phase->update(time, query_size, query_cores, query_time_c); 
            if (finished) {
                running_phases.erase(running_phases.begin()+idx);
            } else {
                idx++;
            }
        }
        if (running_phases.size() == 0) {
            return 1;
        }
        return 0; 
    }

    int updateRR(double time, double *query_size) {
        for (auto phase_p = phases.begin(); phase_p != phases.end();) {
            Phase *cur_phase = &(*phase_p);
            int finished = cur_phase->updateRR(time, query_size); 
            if (finished) {
                phase_p = phases.erase(phase_p);
            } else {
                ++phase_p;
            }
        }
        if (phases.size() == 0) {
            return 1;
        }
        return 0; 

    }

    double getTimeC() { // needs modification to improve efficiency
        double time_c = INFINITY;
        for (int idx = 0; idx < running_phases.size(); idx++) {
            Phase cur_phase = phases[running_phases[idx]];
            time_c = (cur_phase.getTimeC() < time_c) ? cur_phase.getTimeC() : time_c;
        }
        return time_c;
    }

    int checkFinished() {
        if (running_phases.size() == 0 && waiting_phases.size() == 0) {
            return 1;
        }
        return 0;
    }

    int printBlock() {
        std::cout << "running_phases: {" << std::endl;
        for (int idx : running_phases) {
            phases[idx].printPhase();
            std::cout << std::endl;
        }
        std::cout << "}" << std::endl;
        std::cout << "waiting_phases: {" << std::endl;
        for (int idx : waiting_phases) {
            phases[idx].printPhase();
            std::cout << std::endl;
        }
        std::cout << "}" << std::endl;
        return 1;
    }

};

#endif
