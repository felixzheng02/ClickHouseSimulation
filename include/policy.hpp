#ifndef POLICY_H
#define POLICY_H

#include <string>
#include "query.hpp"

enum Policy {
    FCFS = 0,
    SJF = 1,
    SRPT = 2
};

std::string getText(Policy policy);

#endif
