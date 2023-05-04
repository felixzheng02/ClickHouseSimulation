#include "query_generation.hpp"


PhaseGenerator::PhaseGenerator(Distribution<double> *distribution) {
    dist = distribution;
}

PhaseGenerator::PhaseGenerator() {
}

Phase PhaseGenerator::next(int multiprogramming) {
    return {multiprogramming, dist->sample()};
}


Phase PhaseGenerator::next(int multiprogramming, double size) {
    return {multiprogramming, size};
}

Query QueryGenerator::next(double arrival_time) {
    std::vector<Block> blocks = generatePhases1();
    return Query {blocks, arrival_time};
}
QueryGenerator::QueryGenerator(Distribution<double> *arrival_distribution, Distribution<double> *phase_size_distribution, int multiprogramming) : arrival_dist(arrival_distribution), phaseGenerator(phase_size_distribution), multiprogramming(multiprogramming) {
}

QueryGenerator::QueryGenerator() {
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

    std::vector<Block> blocks = generatePhases1();
    return std::make_shared<Query>(blocks, arrival_time);
}

std::vector<Block> QueryGenerator::generatePhases1() {
    std::vector<Block> blocks;
    std::vector<Phase> phases_0, phases_1, phases_2, phases_3, phases_4, phases_5, phases_6, phases_7, phases_8;
    phases_0.push_back(Phase {24, 13.52960691});
    phases_0.push_back(Phase {24, 13.842752106999999});
    phases_0.push_back(Phase {24, 13.518230134});
    phases_0.push_back(Phase {24, 13.655778524});
    phases_0.push_back(Phase {24, 13.479239575000001});
    phases_0.push_back(Phase {24, 13.672424496000001});
    phases_0.push_back(Phase {24, 13.72457811});
    phases_0.push_back(Phase {24, 13.732368807});
    phases_0.push_back(Phase {24, 13.705376594});
    phases_0.push_back(Phase {24, 13.785725577});
    phases_0.push_back(Phase {24, 13.75814241});
    phases_0.push_back(Phase {24, 13.673310320999999});
    phases_0.push_back(Phase {24, 13.941783512});
    phases_0.push_back(Phase {24, 13.870956386});
    phases_0.push_back(Phase {24, 13.504640865});
    phases_0.push_back(Phase {16, 5.837791993});
    blocks.push_back(Block {phases_0, std::vector<int> {1}, 0});
    phases_1.push_back(Phase {25, 13.532176025});
    phases_1.push_back(Phase {27, 13.289140503});
    phases_1.push_back(Phase {26, 13.346008624});
    phases_1.push_back(Phase {24, 11.897828784});
    phases_1.push_back(Phase {25, 12.225341725000002});
    phases_1.push_back(Phase {25, 12.521397325});
    phases_1.push_back(Phase {23, 12.648619655});
    phases_1.push_back(Phase {23, 12.446054906});
    phases_1.push_back(Phase {25, 13.532176025});
    phases_1.push_back(Phase {23, 12.480962005999999});
    phases_1.push_back(Phase {21, 11.371183782});
    phases_1.push_back(Phase {23, 12.325692869999997});
    phases_1.push_back(Phase {19, 9.4320807});
    phases_1.push_back(Phase {21, 11.699650473});
    phases_1.push_back(Phase {24, 12.768951888});
    phases_1.push_back(Phase {23, 11.848107438});
    phases_1.push_back(Phase {24, 12.300862488});
    blocks.push_back(Block {phases_1, std::vector<int> {0}, std::vector<int> {2}});
    phases_2.push_back(Phase {2, 0.0015016040000000001});
    phases_2.push_back(Phase {2, 0.001525706});
    phases_2.push_back(Phase {2, 0.001459213});
    phases_2.push_back(Phase {2, 0.0012588260000000002});
    phases_2.push_back(Phase {1, 6.2347e-05});
    phases_2.push_back(Phase {1, 5.0125e-05});
    phases_2.push_back(Phase {1, 4.3159000000000005e-05});
    phases_2.push_back(Phase {1, 3.8900000000000004e-05});
    phases_2.push_back(Phase {1, 5.0469e-05});
    phases_2.push_back(Phase {1, 5.6088e-05});
    phases_2.push_back(Phase {1, 5.3004e-05});
    phases_2.push_back(Phase {1, 4.5697e-05});
    phases_2.push_back(Phase {1, 5.1489e-05});
    phases_2.push_back(Phase {1, 5.5348999999999995e-05});
    phases_2.push_back(Phase {1, 4.4276999999999996e-05});
    phases_2.push_back(Phase {1, 3.7395e-05});
    blocks.push_back(Block {phases_2, std::vector<int> {1}, std::vector<int> {3}});
    phases_3.push_back(Phase {5, 0.00404055});
    blocks.push_back(Block {phases_3, std::vector<int> {2}, std::vector<int> {4}});
    phases_4.push_back(Phase {4, 0.0029038840000000002});
    blocks.push_back(Block {phases_4, std::vector<int> {3}, std::vector<int> {5}});
    phases_5.push_back(Phase {4, 0.00014153199999999999});
    phases_5.push_back(Phase {1, 5.283e-06});
    phases_5.push_back(Phase {1, 4.540002728});
    blocks.push_back(Block {phases_5, std::vector<int> {4}, std::vector<int> {6}});
    phases_6.push_back(Phase {5, 0.000283195});
    blocks.push_back(Block {phases_6, std::vector<int> {5}, 1});
    phases_7.push_back(Phase {17, 0.356722849});
    phases_7.push_back(Phase {17, 0.34850479399999995});
    phases_7.push_back(Phase {17, 0.35339431699999996});
    phases_7.push_back(Phase {17, 0.35499243599999997});
    phases_7.push_back(Phase {17, 0.353505395});
    phases_7.push_back(Phase {17, 0.353211244});
    phases_7.push_back(Phase {17, 0.345820783});
    phases_7.push_back(Phase {17, 0.352020411});
    phases_7.push_back(Phase {17, 0.350527573});
    phases_7.push_back(Phase {17, 0.353705281});
    phases_7.push_back(Phase {17, 0.36965371199999997});
    phases_7.push_back(Phase {18, 0.390825612});
    phases_7.push_back(Phase {17, 0.34873893499999997});
    phases_7.push_back(Phase {17, 0.339643289});
    phases_7.push_back(Phase {16, 0.323069936});
    phases_7.push_back(Phase {17, 0.349875487});
    blocks.push_back(Block {phases_7, std::vector<int> {8}, 0});
    phases_8.push_back(Phase {253, 4.622980956});
    blocks.push_back(Block {phases_8, std::vector<int> {7}, std::vector<int> {1}}); //changed
    return blocks;    
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
