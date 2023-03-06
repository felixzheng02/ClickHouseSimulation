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

Query *QueryGenerator::nextP() {
    return QueryGenerator::nextP(arrival_dist->sample());
}

Query *QueryGenerator::nextP(double arrival_time) {
    Query *query = new Query();
    double size = 0;
    query->phases.push_back(phaseGenerator->next(4));
    query->phases.push_back(phaseGenerator->next(1));
    query->phases.push_back(phaseGenerator->next(1));
    query->phases.push_back(phaseGenerator->next(1));
    query->phases.push_back(phaseGenerator->next(2));
    for (int i=0; i<query->phases.size(); i++) {
        size += query->phases[i].size;
    }
    query->arrival = arrival_time;
    query->memory = 0;
    query->size = size;
    return query;
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
