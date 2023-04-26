#ifndef QUERY_H
#define QUERY_H

#include "block.hpp"
#include <vector>
#include <iostream>

struct Query {
    std::vector <Block> blocks;
    double arrival; // arrival time
    double size; // remaining processing time
    int cores = 0; // number of cores it is running on
    double time_c = INFINITY;

    Query(std::vector<Block> blocks, double arrival, double size) : blocks(blocks), arrival(arrival), size(size), cores(0) {}
    
    Block *getCurBlock() {
        return &blocks.front();
    }
    
    // return how many cores used
    int allocate(int n_cores) {
        int used_cores = getCurBlock()->allocate(n_cores, &cores, &time_c);
        return used_cores;
    }

    
    // return -1 if the whole Block finishes (can be preempted)
    // return 1 if the whole Query finishes (can be deleted)
    int update(double time) {
        time_c = INFINITY;
        int finished = getCurBlock()->update(time, &size, &cores, &time_c);
        if (finished) {
            if (blocks.size() != 0 && getCurBlock()->waiting_phases.size() == 0) {
                finishBlock();
            }
            if (blocks.size() == 0) {
                return 1;
            }
            return -1;
        }
        return 0;
    }
    
    // return 1 if Query finishes, 0 otherwise
    int updateRR(double time) {
        int finished = getCurBlock()->updateRR(time, &size);
        if (finished) {
            if (getCurBlock()->waiting_phases.size() == 0) {
                finishBlock();
            }
            if (blocks.size() == 0) {
                return 1;
            }
        }
        return 0;
    }

    double getTimeC() {
        return time_c;
    }

    double getTimeCRR() {
        double result = INFINITY;
        for (auto block_p = blocks.begin(); block_p != blocks.end(); ++block_p) {
            for (Phase phase : (*block_p).phases) {
                if (phase.size < result) {
                    result = phase.size;
                }
            }
        }
        return result;
    }

    void finishBlock() {
        blocks.erase(blocks.begin());
    }

    void printQuery() {
        std::cout << "n_cores: " << cores << ", size: " << size << std::endl;
        for (int idx = 0; idx < blocks.size(); idx++) {
            std::cout << "block_" << idx << ": " << std::endl;
            blocks[idx].printBlock();
        }
        std::cout << std::endl;
    }
};

#endif
