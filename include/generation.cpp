#include "generation.hpp"


PhaseGenerator::PhaseGenerator(Distribution<double> *distribution) {
    dist = distribution;
}

Phase PhaseGenerator::next(int multiprogramming) {
    return {multiprogramming, dist->sample()};
}


Phase PhaseGenerator::next(int multiprogramming, double size) {
    return {multiprogramming, size};
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

    std::vector<Phase> phases;
    double size = generatePhases1(&phases);
    return std::make_shared<Query>(phases, arrival_time, size, 0, 0);
}

double QueryGenerator::generatePhases1(std::vector<Phase> *phases) {
    double size = 0;
    Phase cur_phase = phaseGenerator.next(1);
    phases->push_back(cur_phase);
    size += cur_phase.size;
    cur_phase = phaseGenerator.next(multiprogramming);
    phases->push_back(cur_phase);
    size += cur_phase.size;
    cur_phase = phaseGenerator.next(multiprogramming);
    phases->push_back(cur_phase);
    size += cur_phase.size;
    cur_phase = phaseGenerator.next(1);
    phases->push_back(cur_phase);
    size += cur_phase.size;
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
