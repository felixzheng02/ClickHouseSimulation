#ifndef BLOCK_H 
#define BLOCK_H 

#include <vector>
#include <random>
#include "phase.hpp"

struct Block {
    std::vector<Phase> phases; // never access phases directly, use running_phases and waiting_phases
    std::vector<Phase *> running_phases;
    std::vector<Phase *> waiting_phases;
    double size = 0;

    Block(std::vector<Phase> phases) : phases(phases) {
        for (Phase phase : phases) {
            waiting_phases.push_back(&phase);
            size += phase.size;
        }
    }
    
    // return how many cores left
    int allocate(int n_cores) {
        while (n_cores > 0 && waiting_phases.size() > 0) {
            n_cores = allocateOnePhase(n_cores);
        }
        return n_cores;
    }
    

    int allocateOnePhase(int n_cores) {

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        
        std::uniform_int_distribution<> distr_phase(0, waiting_phases.size()-1);
        int index = distr_phase(gen);
        Phase *phase_p = waiting_phases[index];

        std::uniform_int_distribution<> distr_cores(1, std::min(n_cores, phase_p->multiprogramming));
        int cores = distr_cores(gen);

        phase_p->allocate(cores); 
        n_cores -= cores;
        running_phases.push_back(phase_p);
        waiting_phases.erase(waiting_phases.begin()+index);
        
        return n_cores;
    }


    // return Block size decrement
    double update(double time) {
        double size_dec = 0;
        for (auto phase_p = running_phases.begin(); phase_p != running_phases.end();) {
            Phase *cur_phase_p = *phase_p;
            double cur_size_dec = cur_phase_p->update(time); 
            size_dec += cur_size_dec;
            if (std::abs(cur_phase_p->size) <= 1e-7f) {
                phase_p = running_phases.erase(phase_p);
            } else {
                ++phase_p;
            }
        }
        size -= size_dec;
        return size_dec; 
    }

    double getTimeC() { // needs modification to improve efficiency
        double time_c = INFINITY;
        for (auto phase_p = running_phases.begin(); phase_p != running_phases.end(); ++phase_p) {
            time_c = ((*phase_p)->getTimeC() < time_c) ? (*phase_p)->getTimeC() : time_c;
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
        for (Phase *phase_p : running_phases) {
            phase_p->printPhase();
            std::cout << std::endl;
        }
        std::cout << "}" << std::endl;
        std::cout << "waiting_phases: {" << std::endl;
        for (Phase *phase_p : waiting_phases) {
            phase_p->printPhase();
            std::cout << std::endl;
        }
        std::cout << "}" << std::endl;
        return 1;
    }

};

#endif
