struct Phase {
    int multiprogramming = 1; // 1: inelastic; more than 1: elastic
    double size; // remaining processing time given one CPU core
};
