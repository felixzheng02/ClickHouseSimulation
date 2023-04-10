#include "generation.hpp"


PhaseGenerator::PhaseGenerator(Distribution<double> *distribution) {
    dist = distribution;
}

Phase PhaseGenerator::next(int multiprogramming) {
    return {multiprogramming, dist->sample()};
}

QueryGenerator::QueryGenerator(Distribution<double> *arrival_distribution, Distribution<double> *phase_size_distribution) {
	arrival_dist = arrival_distribution; 
    phaseGenerator = new PhaseGenerator(phase_size_distribution);
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
    double size = 0;
    phases.push_back(phaseGenerator->next(64));
    phases.push_back(phaseGenerator->next(1));
    for (int i=0; i<phases.size(); i++) {
        size += phases[i].size;
    }
    return std::make_shared<Query>(phases, arrival_time, size, 0, 0);
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
