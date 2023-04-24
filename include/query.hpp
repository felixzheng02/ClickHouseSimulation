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
        int used_cores = getCurBlock()->allocate(n_cores);
        cores += used_cores;
        return used_cores;
    }

    
    // return 1 if Block finishes (can be preempted)
    int update(double time) {
        double cur_size_dec = getCurBlock()->update(time);
        size -= cur_size_dec;
        if (std::abs(getCurBlock()->size) <= 1e-7f) {
            blocks.erase(blocks.begin());
            return 1;
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
        std::cout << "n_cores: " << cores << ", size: " << size << ", blocks: ";
        for (Block block : blocks) {
            block.printBlock();
        }
        std::cout << std::endl;
    }
};

#endif
