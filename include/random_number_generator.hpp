#pragma once

#include <random>

class random_number_generator {
public:
    random_number_generator(double _min, double _max);
    double generate();

private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<double> m_real_dist;
};
