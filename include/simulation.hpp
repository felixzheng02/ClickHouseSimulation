#include <cmath>
#include "generation.hpp"


class Simulation {
    protected:
        double time = 0.0;
        int n_jobs = 0;
        double time_c = INFINITY; // time until next completion
        double time_a; // time until next arrival
        QueryGenerator *queryGenerator;
    public:
        Simulation(); 
        void initialize();
        double run();
};
