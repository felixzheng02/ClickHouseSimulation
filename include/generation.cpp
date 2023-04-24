#include "generation.hpp"


PhaseGenerator::PhaseGenerator(Distribution<double> *distribution) {
    dist = distribution;
}

Phase PhaseGenerator::next(int multiprogramming) {
    return {multiprogramming, 0, dist->sample()};
}


Phase PhaseGenerator::next(int multiprogramming, double size) {
    return {multiprogramming, 0, size};
}


QueryGenerator::QueryGenerator(Distribution<double> *arrival_distribution, Distribution<double> *phase_size_distribution, int multiprogramming) : arrival_dist(arrival_distribution), phaseGenerator(phase_size_distribution), multiprogramming(multiprogramming) {
}

std::shared_ptr<Query> QueryGenerator::nextP() {
    return QueryGenerator::nextP(arrival_dist->sample());
}

std::shared_ptr<Query> QueryGenerator::nextP(double arrival_time) {
    /*
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(25, 63); // define the range
    */

    std::vector<Block> blocks;
    double size = generatePhases1(&blocks);
    return std::make_shared<Query>(blocks, arrival_time, size);
}

double QueryGenerator::generatePhases1(std::vector<Block> *blocks) {
    double size = 0;
    std::vector<Phase> phases_1, phases_2;
    Phase phase = phaseGenerator.next(multiprogramming);
    phases_1.push_back(phase);
    size += phase.size;
    phase = phaseGenerator.next(1);
    phases_1.push_back(phase);
    size += phase.size;
    Block block { phases_1 };
    blocks->push_back(block);
    phase = phaseGenerator.next(1);
    phases_2.push_back(phase);
    size += phase.size;
    Block block_2 { phases_2 };
    blocks->push_back(block_2);
    
    return size;
}


Distribution<double> *QueryGenerator::getArrivalDist() {
    return arrival_dist;
}

//void MemoryBoundGenerator::generate_memory(unsigned int classes, double L, double R, double memory_alpha) {
//	BoundedParetoDistribution dist {L, R, memory_alpha};
//	for (int i = 0; i < classes; ++ i) {
//		memory[i] = dist.sample();
//	}
//}

//Query MemoryBoundGenerator::next() {
//	int num = zipf_dist -> sample();
//	return {
//		last_arrival += arrival_dist -> sample(), 
//								 memory[num],
//								 size_dist -> sample() / mu,
//								 num
//	};
//}
