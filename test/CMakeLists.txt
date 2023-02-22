cmake_minimum_required(VERSION 3.21)

project(ClickHouseSimulationTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(../include)
set(SOURCES test_simulation.cpp ../include/generation.hpp ../include/phase.hpp ../include/query.hpp ../include/simulation.hpp ../include/generation.cpp ../include/simulation.cpp)
add_executable(test_simulation ${SOURCES})

#target_link_libraries(test_simulation PUBLIC simulation_lib)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
