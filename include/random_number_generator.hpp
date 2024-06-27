#ifndef MACHINE_LEARNING_RANDOM_NUMBER_GENERATOR_HPP
#define MACHINE_LEARNING_RANDOM_NUMBER_GENERATOR_HPP
#include <random>
class Random_Number_Generator {
public:
    Random_Number_Generator(double _min, double _max);
    double generate();
private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<double> m_real_dist;
};
#endif //MACHINE_LEARNING_RANDOM_NUMBER_GENERATOR_HPP