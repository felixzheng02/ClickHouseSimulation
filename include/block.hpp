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
    double size = 0;

    Block(std::vector<Phase> phases) : phases(phases) {
        for (int idx = 0; idx < phases.size(); idx++) {
            waiting_phases.push_back(idx);
            size += phases[idx].size;
        }
    }

    int initialize() {
        for (int idx = 0; idx < phases.size(); idx++) {
            waiting_phases.push_back(idx);
            size += phases[idx].size;
        }
        return 1;
    }
    
    // return how many cores used
    int allocate(int n_cores) {
        int left_cores = n_cores;
        std::vector<int> allocated_phases;
        while (left_cores > 0 && running_phases.size() != phases.size()) {
            for (auto idx_p = waiting_phases.begin(); idx_p != waiting_phases.end();) {
                if (left_cores > 0) {
                    left_cores = allocatePhase(idx_p, left_cores, &waiting_phases, &allocated_phases, &running_phases);
                }
            }
            left_cores = allocateAlloc(left_cores, &allocated_phases, &running_phases);
        }
        running_phases.insert(running_phases.end(), allocated_phases.begin(), allocated_phases.end());
        return n_cores - left_cores;
    }
    

    int allocatePhase(std::vector<int>::iterator idx_p, int n_cores, std::vector<int> *low, std::vector<int> *mid, std::vector<int> *up) {

        Phase *phase = &phases[(*low)[*idx_p]];

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr_cores(1, std::min(n_cores, phase->multiprogramming - phase->cores));
        int cores = distr_cores(gen);

        phase->allocate(cores); 
        n_cores -= cores;
        if (phase->cores == phase->multiprogramming) {
            up->push_back((*low)[*idx_p]);
        } else {
            mid->push_back((*low)[*idx_p]);
        }
        idx_p = low->erase(idx_p);
        return n_cores;
    }

    int allocateAlloc(int n_cores, std::vector<int> *low, std::vector<int> *up) {
        
        std::vector<int> tmp_idx;
        for (auto idx_p = low->begin(); idx_p != low->end();) {
            if (n_cores == 0) return n_cores;
            Phase *phase = &phases[(*low)[*idx_p]];

            std::random_device rd; // obtain a random number from hardware
            std::mt19937 gen(rd()); // seed the generator
            std::uniform_int_distribution<> distr_cores(1, std::min(n_cores, phase->multiprogramming - phase->cores));
            int cores = distr_cores(gen);

            phase->allocate(cores); 
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

    // return Block size decrement
    double update(double time) {
        double size_dec = 0;
        for (int idx = 0; idx < running_phases.size();) {
            Phase cur_phase = phases[waiting_phases[idx]];
            double cur_size_dec = cur_phase.update(time); 
            size_dec += cur_size_dec;
            if (std::abs(cur_phase.size) <= 1e-7f) {
                running_phases.erase(running_phases.begin()+idx);
            } else {
                idx++;
            }
        }
        size -= size_dec;
        return size_dec; 
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
