#include "../include/simulation.hpp"
#include <iostream>

int main() {
    Simulation *simulation = new Simulation();
    simulation->initialize();
    for (int i=0; i<10; i++) {
        simulation->run();
    }
    return 1;
}
