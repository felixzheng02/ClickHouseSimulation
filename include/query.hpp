#ifndef QUERY_H
#define QUERY_H

#include "block.hpp"
#include <vector>
#include <iostream>

struct Query {
    std::vector<Block> blocks;
    std::vector<int> finished_blocks;
    std::vector<int> full_blocks;
    std::vector<int> running_blocks;
    std::vector<int> ready_blocks;
    std::vector<int> waiting_blocks;
    double arrival; // arrival time
    double size; // remaining processing time
    int cores = 0; // number of cores it is running on
    double time_c = INFINITY;
    UniformIntDistribution uniform_int_distribution;

    Query(std::vector<Block> blocks, double arrival, double size) : blocks(blocks), arrival(arrival), size(size), cores(0) {
        for (int idx = 0; idx < blocks.size(); idx++) {
            waiting_blocks.push_back(idx);
            Block block = blocks[idx];
            if (block.before_blocks.size() == 0) {
                waiting_blocks.erase(waiting_blocks.begin() + waiting_blocks.size()-1);
                ready_blocks.push_back(idx);
            }
        }
    }

    Query(std::vector<Block> blocks, double arrival) : blocks(blocks), arrival(arrival), cores(0) {
        size = 0;
        for (int idx = 0; idx < blocks.size(); idx++) {
            waiting_blocks.push_back(idx);
            Block block = blocks[idx];
            for (Phase phase : block.phases) {
               size += phase.size;
            }
            if (block.before_blocks.size() == 0) {
                waiting_blocks.erase(waiting_blocks.begin() + waiting_blocks.size()-1);
                ready_blocks.push_back(idx);
            }
        }
    }
    
    // return how many cores used
    int allocate(int n_cores) {
        int used_cores = 0;
        while (used_cores < n_cores && ready_blocks.size()+running_blocks.size()>0) {
            int index = uniform_int_distribution.sample(0, ready_blocks.size()+running_blocks.size()-1);
            Block *block;
            if (index < ready_blocks.size()) {
                block = &blocks[ready_blocks[index]];
            } else {
                block = &blocks[running_blocks[index-ready_blocks.size()]];
            }
            int cur_used_cores = block->allocate(1, &cores, &time_c);
            if (cur_used_cores > 0) {
                if (index < ready_blocks.size()) {
                    running_blocks.push_back(ready_blocks[index]);
                    ready_blocks.erase(ready_blocks.begin() + index);
                }
                used_cores += cur_used_cores;
            } else {
                if (index >= ready_blocks.size() && block->running_phases.size() == 0 && block->waiting_phases.size() == 0) {
                    full_blocks.push_back(running_blocks[index-ready_blocks.size()]);
                    running_blocks.erase(running_blocks.begin() + index - ready_blocks.size());
                }
            }
        }
        return used_cores;
    }

    int deallocate() {
        for (auto idx_p = full_blocks.begin(); idx_p != full_blocks.end();) {
            Block *block = &blocks[*idx_p];
            block->deallocate();
            ready_blocks.push_back(*idx_p);
            idx_p = full_blocks.erase(idx_p);
        }
        for (auto idx_p = running_blocks.begin(); idx_p != running_blocks.end();) {
            Block *block = &blocks[*idx_p];
            block->deallocate();
            ready_blocks.push_back(*idx_p);
            idx_p = running_blocks.erase(idx_p);
        }
        cores = 0;
        time_c = INFINITY;
        return 1;
    }

    
    // return -1 if the all running Blocks finishes (can be preempted)
    // return 1 if the whole Query finishes (can be deleted)
    int update(double time) {
        time_c = INFINITY;
        int result = 0;
        updateBlocks(&full_blocks, time, &size, &cores, &time_c);
        updateBlocks(&running_blocks, time, &size, &cores, &time_c);
        if (full_blocks.size() == 0 && running_blocks.size() == 0) {
            result = -1;
            if (finished_blocks.size() == blocks.size()) {
                result = 1;
            }
        }
        return result;
    }

    int updateBlocks(std::vector<int> *block_lst, double time, double *size, int *cores, double *time_c) {
        for (auto idx_p = block_lst->begin(); idx_p != block_lst->end();) {
            int index = *idx_p;
            Block *cur_block = &blocks[index];
            int finished = cur_block->update(time, size, cores, time_c);
            if (finished == 1) {
                // cur_block finishes 
                idx_p = block_lst->erase(idx_p);
                finished_blocks.push_back(index);
                findReady();
            } else {
                ++idx_p;
            }
        }
        return 1;
    }

    // move ready block indexs from waiting_blocks to ready_blocks
    int findReady() {
        for (auto idx_p = waiting_blocks.begin(); idx_p != waiting_blocks.end();) {
            int ready = 1;
            for (int idx : blocks[*idx_p].before_blocks) {
                if (std::find(finished_blocks.begin(), finished_blocks.end(), idx) == finished_blocks.end()) {
                    ready = 0;
                    break;
                }
            }
            if (ready) {
                ready_blocks.push_back(*idx_p);
                idx_p = waiting_blocks.erase(idx_p);
            } else {
                ++idx_p;
            }
        }
        return 1;
    }
    
    // return 1 if Query finishes, 0 otherwise
    //int updateRR(double time) {
      //  int finished = getCurBlock()->updateRR(time, &size);
        //if (finished) {
            //if (getCurBlock()->waiting_phases.size() == 0) {
                //finishBlock();
            //}
            //if (blocks.size() == 0) {
                //return 1;
            //}
        //}
        //return 0;
    //}

    double getTimeC() {
        return time_c;
    }


    void printQuery(int details = 0) {
        if (details == 0) {
            std::cout << "n_cores: " << cores << ", size: " << size << std::endl;
            for (int idx = 0; idx < full_blocks.size(); idx++) {
                std::cout << "block_" << idx << ": " << std::endl;
                blocks[full_blocks[idx]].printBlock();
            }
            for (int idx = 0; idx < running_blocks.size(); idx++) {
                std::cout << "block_" << idx << ": " << std::endl;
                blocks[running_blocks[idx]].printBlock();
            }
            std::cout << std::endl;
        } else {
            std::cout << "n_cores: " << cores << ", size: " << size << std::endl;
            for (int idx = 0; idx < blocks.size(); idx++) {
                std::cout << "block_" << idx << ": " << std::endl;
                blocks[idx].printBlock();
            }
            std::cout << std::endl;
        }
    }

    double testSpeedUp(int n_cores) {
        double time = 0;
        int finished = 0;
        int used_cores = allocate(n_cores);
        int left_cores = n_cores - used_cores;
        while (finished != 1) {
            time += time_c;
            left_cores += cores;
            finished = update(time_c);
            left_cores -= cores;
            left_cores -= allocate(left_cores);
            if ((time_c == INFINITY || time_c == -INFINITY) && finished == 0) {
                std::cout << "";
            }
            // if (getTimeC() == INFINITY) throw std::string("exception ! error");
        }
        return time;
    }
};

#endif
