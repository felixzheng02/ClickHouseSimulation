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

    Query(std::vector<Block> blocks, double arrival, double size) : blocks(blocks), arrival(arrival), size(size), cores(0) {}
    
    Block *getCurBlock() {
        return &blocks.front();
    }
    
    // return how many cores used
    int allocate(int n_cores) {
        int used_cores = getCurBlock()->allocate(n_cores, &cores);
        return used_cores;
    }

    
    // return -1 if the whole Block finishes (can be preempted)
    // return 1 if the whole Query finishes (can be deleted)
    int update(double time) {
        int finished = getCurBlock()->update(time, &size, &cores);
        if (finished) {
            if (getCurBlock()->waiting_phases.size() == 0) {
                finishBlock();
            }
            if (blocks.size() == 0) {
                return 1;
            }
            return -1;
        }
        return 0;
    }

    double getTimeC() {
        return getCurBlock()->getTimeC();
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
