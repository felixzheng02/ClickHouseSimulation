#include "generation.hpp"
#include "query.hpp"

class PhaseGenerator {
    Distribution<double> *dist;
    public:
    PhaseGenerator(Distribution<double> *distribution);
    PhaseGenerator();
    Phase next(int multiprogramming);
    Phase next(int multiprogramming, double size);
};

class QueryGenerator {
    Distribution<double> *arrival_dist;
    PhaseGenerator phaseGenerator;
    int multiprogramming = 16;
    public:
	QueryGenerator(Distribution<double> *arrival_dist, Distribution<double> *phase_size_dist, int multiprogramming);
    QueryGenerator();
    Query next(double arrival_time);
    std::shared_ptr<Query> nextP(); 
    std::shared_ptr<Query> nextP(double arrival_time);  
    Distribution<double> *getArrivalDist();
    std::vector<Block> generatePhases1();
};

