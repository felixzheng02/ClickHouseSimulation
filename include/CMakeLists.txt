add_library(simulation_lib simulation.cpp generation.cpp)

target_include_directories(simulation_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
