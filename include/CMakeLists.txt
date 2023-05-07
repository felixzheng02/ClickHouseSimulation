add_library(simulation_lib simulation.cpp query_generation.cpp generation.hpp)

target_include_directories(simulation_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#set(CMAKE_BUILD_TYPE Debug)
