#ifndef GENERATION_H 
#define GENERATION_H

#include <random>
#include <fstream>
#include <cmath>
#include <array>
#include "query.hpp"

template <class T>
class Distribution {
    protected:
	unsigned int seed;
	std::mt19937 prg;
    public:
	Distribution(unsigned int seed) : seed(seed) {
		prg = std::mt19937 {seed};
	}
	virtual T sample() = 0;
};

class ExponentialDistribution : public Distribution<double> {
    std::exponential_distribution<double> dist;
    public:
	ExponentialDistribution(unsigned int seed, double lambda) : Distribution<double>(seed) {
		dist = std::exponential_distribution<double> {lambda};
	}
	ExponentialDistribution(double lambda) : ExponentialDistribution((std::random_device())(), lambda) {}
	double sample() override {
		return dist(prg);
	}
};

class UniformDistribution : public Distribution<double> {
    protected:
	std::uniform_real_distribution<double> dist;
    public:
	UniformDistribution(unsigned int seed, double l = 0.0, double r = 1.0) : Distribution<double>(seed) {
		dist = std::uniform_real_distribution<double> {l, r};
	}
	UniformDistribution(double l = 0.0, double r = 1.0) : UniformDistribution((std::random_device())(), l, r) {}
	double sample() override {
		return dist(prg);
	}
};

class ParetoDistribution : public UniformDistribution {
	double alpha;
    public:
	ParetoDistribution(unsigned int seed, double alpha = 1.0) : UniformDistribution(seed), alpha(alpha) {}
	ParetoDistribution(double alpha = 1.0) : UniformDistribution(), alpha(alpha) {}
	double sample() override {
		double u = UniformDistribution::sample();
		return std::pow(1 - u, -1 / alpha) * (alpha - 1.0) / alpha;
	}
};

class BoundedParetoDistribution : public UniformDistribution {
	double L, H, alpha;
    public:
	BoundedParetoDistribution(unsigned int seed, double L, double H, double alpha) : UniformDistribution(seed), L(L), H(H), alpha(alpha) {}
	BoundedParetoDistribution(double L, double H, double alpha) : UniformDistribution(), L(L), H(H), alpha(alpha) {}
	double sample() override {
		double u = UniformDistribution::sample();
		double nom = u * std::pow(H, alpha) - u * std::pow(L, alpha) - std::pow(H, alpha);
		double denom = std::pow(H, alpha) * std::pow(L, alpha);
		return std::pow(-nom / denom, -1.0 / alpha);
	}
};

class ZipfianDistribution : public Distribution<int> {
	std::discrete_distribution<int> dist;
    public:
	ZipfianDistribution(unsigned int seed, unsigned int n, double alpha) : Distribution<int>(seed) {
		std::vector<double> init;
		for (int i = 0; i < n; ++ i) {
			init.push_back(std::pow(i + 1.0, -alpha));
		}
		dist = std::discrete_distribution<int>(init.begin(), init.end());
	}
	ZipfianDistribution(unsigned int n, double alpha) : ZipfianDistribution((std::random_device())(), n, alpha) {}
	int sample() override {
		return dist(prg);
	}
};

template <class T>
class Generator {
    public:
	Generator() {}
};

class PhaseGenerator : public Generator<Phase> {
    Distribution<double> *dist;
    public:
    PhaseGenerator(Distribution<double> *distribution);
    Phase next(int multiprogramming);
};

class QueryGenerator : public Generator<Query> {
    Distribution<double> *arrival_dist;
    PhaseGenerator *phaseGenerator;
    public:
	QueryGenerator(Distribution<double> *arrival_dist, Distribution<double> *phase_size_dist);
    Query *nextP(); 
    Query *nextP(double arrival_time);  
    Distribution<double> *getArrivalDist();
};

//class MemoryBoundGenerator : public Generator {
//	Distribution<double> *arrival_dist, *size_dist;
//	Distribution<int> *zipf_dist;
//	double last_arrival = 0;
//	double mu;
//	double *memory;
//    public:
//	MemoryBoundGenerator(double lambda, double mu, double G, double pareto_alpha, int classes, unsigned long seeds[], double L = 1.0, double R = 12.0, double memory_alpha = 1.5) : mu(mu){
//		arrival_dist = new ExponentialDistribution(lambda);
//		size_dist = new ParetoDistribution(pareto_alpha);
//		zipf_dist = new ZipfianDistribution(classes, 0.9);
//		memory = new double[classes];
//		generate_memory(classes, L, R, memory_alpha);
//	}
//	void generate_memory(unsigned int classes, double L, double R, double memory_alpha);
//	Query next() override;
//};

//typedef MemoryBoundGenerator ZipfGenerator;

#endif
