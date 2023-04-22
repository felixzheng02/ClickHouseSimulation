#ifndef QUERY_H
#define QUERY_H

#include "block.hpp"
#include <vector>
#include <iostream>

struct Query {
    std::vector <Block> blocks;
    double arrival; // arrival time
    double size; // remaining processing time
    int n_cores = 0; // number of cores it is running on

    Query(std::vector<Block> blocks, double arrival, double size, int n_cores) : blocks(blocks), arrival(arrival), size(size), n_cores(n_cores) {}
    
    Block getNextBlock() {
        return blocks.front();
    }


    void finishBlock() {
        blocks.erase(blocks.begin());
    }

    void printQuery() {
        std::cout << "n_cores: " << n_cores << ", size: " << size << ", blocks: ";
        for (Block block : blocks) {
            block.printBlock();
        }
        std::cout << std::endl;
    }
};

#endif
