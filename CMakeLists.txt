cmake_minimum_required(VERSION 3.21)

# set the project name
project(ClickHouseSimulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include)
add_subdirectory(src)

file(GLOB SOURCES "src/*.cpp")
add_executable(ClickHouseSimulation ${SOURCES})
target_link_libraries(ClickHouseSimulation PUBLIC simulation_lib)

# target_include_directories(ClickHouseSimulation PUBLIC
#                           "${PROJECT_BINARY_DIR}"
#                           "${PROJECT_SOURCE_DIR}/simulation_lib"
#                           )
