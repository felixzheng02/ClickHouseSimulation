#ifndef BLOCK_H 
#define BLOCK_H 

#include <vector>
#include <random>
#include "phase.hpp"

struct Block {
    std::vector<Phase> phases; // never access phases directly, use running_phases and waiting_phases
    std::vector<Phase *> running_phases;
    std::vector<Phase *> waiting_phases;

    Block(std::vector<Phase> phases) : phases(phases) {
        for (Phase phase : phases) {
            waiting_phases.push_back(&phase);
        }
    }

    int allocate(int n_cores) {
        while (n_cores > 0) {
            n_cores = allocateOnePhase(n_cores);
        }
        return 1;
    }

    int allocateOnePhase(int n_cores) {

        std::random_device rd; // obtain a random number from hardware
        std::mt19937 gen(rd()); // seed the generator
        std::uniform_int_distribution<> distr_phase(0, waiting_phases.size()-1); // define the range
        std::uniform_int_distribution<> distr_cores(1, n_cores);

        int index = distr_phase(gen);
        int cores = distr_cores(gen);
        Phase *phase_p = waiting_phases[index];
        phase_p->allocate(cores); 
        n_cores -= cores;
        running_phases.push_back(phase_p);
        waiting_phases.erase(waiting_phases.begin()+index);
        
        return n_cores;
    }


    // if some phase finishes, return 1
    int update(double time) {
        int result = 0;
        for (auto phase_p = running_phases.begin(); phase_p != running_phases.end();) {
            int finished = (*phase_p)->update(time); 
            if (finished) {
                result = 1;
                phase_p = running_phases.erase(phase_p);
            }
        }
        return result;
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
