#include "generation.hpp"


PhaseGenerator::PhaseGenerator() {
    dist = new UniformDistribution {0.0, 1.0};
}

Phase PhaseGenerator::next() {
    double tmp = dist->sample();
    return {1, dist->sample()};
}

QueryGenerator::QueryGenerator(double lambda, double pareto_alpha) {
	arrival_dist = new ExponentialDistribution(lambda);
	size_dist = new ParetoDistribution(pareto_alpha);
    phaseGenerator = new PhaseGenerator();
}

Query QueryGenerator::next() {
    std::vector<Phase> phases;
    double size = 0;
    for (int i=0; i<3; i++) {
        Phase tmp = phaseGenerator->next();
        phases.push_back(phaseGenerator->next());
        size += phases.back().size;
    }
    return {phases, arrival_dist->sample(), size, 0};
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
