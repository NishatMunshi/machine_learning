#include "../include/random_number_generator.hpp"
#include <cfloat>

random_number_generator::random_number_generator(double _min, double _max) {
    std::random_device random_device;
    m_generator = std::mt19937{random_device()};
    m_real_dist = std::uniform_real_distribution{_min, std::nextafter(_max, DBL_MAX)};
}

double random_number_generator::generate() {
    return m_real_dist(m_generator);
}
