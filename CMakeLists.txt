cmake_minimum_required(VERSION 3.28)
project(machine_learning)

set(CMAKE_CXX_STANDARD 26)

include_directories(
        "E:\\programming_tools\\SFML\\SFML_Sources\\include"
)

add_executable(
        machine_learning
        main.cpp
        include/matrix.hpp
        matrix.cpp
        include/neural_network.hpp
        neural_network.cpp
        include/random_number_generator.hpp
        random_number_generator.cpp
)
