#ifndef BLOCK_H 
#define BLOCK_H 

#include <type_traits>
#include <vector>
#include <random>
#include "phase.hpp"

struct Block {
    std::vector<Phase> phases; // never change phases directly, use running_phases and waiting_phases
    std::vector<int> full_phases;
    std::vector<int> running_phases;
    std::vector<int> waiting_phases;
    std::vector<int> before_blocks;
    std::vector<int> next_blocks;
    UniformIntDistribution uniform_int_distribution;

    Block(std::vector<Phase> phases, std::vector<int> before_blocks, std::vector<int> next_blocks) : phases(phases), before_blocks(before_blocks), next_blocks(next_blocks) {
        for (int idx = 0; idx < phases.size(); idx++) {
            waiting_phases.push_back(idx);
        }
    }
    
    Block(std::vector<Phase> phases, std::vector<int> blocks, int before) : phases(phases) {
        if (before) {
            before_blocks = blocks;
        } else {
            next_blocks = blocks;
        }
        for (int idx = 0; idx < phases.size(); idx++) {
            waiting_phases.push_back(idx);
        }
    }

    // return how many cores used
    int allocate(int n_cores, int *query_cores, double *query_time_c) {
        if (waiting_phases.size() + running_phases.size() == 0) return 0;

        int index = uniform_int_distribution.sample(0, waiting_phases.size()+running_phases.size()-1);
        int used_cores = 0;
        Phase *cur_phase;
        if (index < waiting_phases.size()) {
            cur_phase = &phases[waiting_phases[index]];
        } else {
            cur_phase = &phases[running_phases[index-waiting_phases.size()]];
        }
        int cur_used_cores = cur_phase->allocate(1, query_cores, query_time_c); 
        if (cur_used_cores > 0) {
            if (index < waiting_phases.size()) {
                    running_phases.push_back(waiting_phases[index]);
                    waiting_phases.erase(waiting_phases.begin() + index);
                }
                used_cores += cur_used_cores;
        } else {
            if (index >= waiting_phases.size()) {
                full_phases.push_back(running_phases[index-waiting_phases.size()]);
                running_phases.erase(running_phases.begin()+index-waiting_phases.size());
            }
        }

        return used_cores;
    }

    int deallocate() {
        for (auto idx_p = full_phases.begin(); idx_p != full_phases.end();) {
            Phase *cur_phase = &phases[*idx_p];
            cur_phase->deallocate();
            waiting_phases.push_back(*idx_p);
            idx_p = full_phases.erase(idx_p);
        }
        for (auto idx_p = running_phases.begin(); idx_p != running_phases.end();) {
            Phase *cur_phase = &phases[*idx_p];
            cur_phase->deallocate();
            waiting_phases.push_back(*idx_p);
            idx_p = running_phases.erase(idx_p);
        }
        return 1;
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

    // return -1 if all current running Phases are finished
    // return 1 if the Block finishes
    int update(double time, double *query_size, int *query_cores, double *query_time_c) {
        for (auto idx_p = running_phases.begin(); idx_p != running_phases.end();) {
            Phase *cur_phase = &phases[*idx_p];
            int finished = cur_phase->update(time, query_size, query_cores, query_time_c); 
            if (finished) {
                idx_p = running_phases.erase(idx_p);
            } else {
                ++idx_p;
            }
        }
        for (auto idx_p = full_phases.begin(); idx_p != full_phases.end();) {
            Phase *cur_phase = &phases[*idx_p];
            int finished = cur_phase->update(time, query_size, query_cores, query_time_c); 
            if (finished) {
                idx_p = full_phases.erase(idx_p);
            } else {
                ++idx_p;
            }
        }

        if (running_phases.size() == 0 && full_phases.size() == 0) {
            if (waiting_phases.size() == 0) {
                return 1;
            }
            return -1;
        }
        return 0; 
    }


    double getTimeC() { // needs modification to improve efficiency
        double time_c = INFINITY;
        for (auto idx_p = running_phases.begin(); idx_p != running_phases.end();) {
            Phase cur_phase = phases[*idx_p];
            time_c = (cur_phase.getTimeC() < time_c) ? cur_phase.getTimeC() : time_c;
        }
        for (auto idx_p = full_phases.begin(); idx_p != full_phases.end();) {
            Phase cur_phase = phases[*idx_p];
            time_c = (cur_phase.getTimeC() < time_c) ? cur_phase.getTimeC() : time_c;
        }
        return time_c;
    }

    int checkFinished() {
        if (full_phases.size() == 0 && running_phases.size() == 0 && waiting_phases.size() == 0) {
            return 1;
        }
        return 0;
    }

    int printBlock() {
        std::cout << "before_blocks: ";
        for (int idx : before_blocks) {
            std::cout << idx << ", ";
        }
        std::cout << "next_blocks: ";
        for (int idx : next_blocks) {
            std::cout << idx << ", ";
        }
        std::cout << std::endl;
        std::cout << "full_phases: {" << std::endl;
        for (int idx : full_phases) {
            phases[idx].printPhase();
            std::cout << std::endl;
        }
        std::cout << "}" << std::endl;
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
