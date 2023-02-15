#include "../include/simulation.hpp"
#include <iostream>

int main() {
    Simulation *simulation = new Simulation();
    double result = simulation->run();
    std::cout << result << std::endl;
    return 1;
}
