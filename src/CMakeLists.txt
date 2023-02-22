add_executable(main main.cpp)

target_link_libraries(main simulation_lib)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
