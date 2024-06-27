#ifndef MACHINE_LEARNING_NEURAL_NETWORK_HPP
#define MACHINE_LEARNING_NEURAL_NETWORK_HPP
#include <vector>
#include <cstdint>
#include "matrix.hpp"
class Neural_Network {
public:
    explicit Neural_Network(std::vector<size_t> const&_topology);
    std::vector<double> feed_forward(std::vector<double> const& _input);
    void backprop(std::vector<double> const&_expected);
    static double mean_squared_error(std::vector<double> const &_result, std::vector<double> const &_expected);

private:
    std::vector<Matrix> A;
    std::vector<Matrix> Z;
    std::vector<Matrix> W;
    std::vector<Matrix> B;

    static Matrix activation_function(Matrix const& _mat);
    static double activation_function(double _x);
    static double activation_function_derivative(double _x);

    double del_C_del_A(size_t _layerIndex, size_t _neuronIndex, std::vector<double> const& _expected);
    double del_C_del_W(size_t _layerIndex, size_t _k, double _del_c_del_b);
    double del_C_del_B(size_t _layerIndex, size_t _j, std::vector<double> const& _expected);

    static double eta;
};
#endif //MACHINE_LEARNING_NEURAL_NETWORK_HPP
