#pragma once

#include "matrix.hpp"

class neural_network {
public:
    explicit neural_network(std::vector<size_t> const &_topology);

    std::vector<double> feed_forward(std::vector<double> const &_input);
    void backprop(std::vector<double> const &_expected);

    static double mean_squared_error(std::vector<double> const &_result, std::vector<double> const &_expected);

private:
    std::vector<matrix> A;
    std::vector<matrix> Z;
    std::vector<matrix> W;
    std::vector<matrix> B;

    // memoize the derivatives of cost function
    std::vector<matrix> delC_delA;
    std::vector<matrix> delA_delZ;
    std::vector<matrix> delC_delW;
    std::vector<matrix> delC_delB;

    static matrix activation_function(matrix const &_mat);
    static double activation_function(double _x);
    static double activation_function_derivative(double _x);

    void calculate_gradient(std::vector<double> const &_expected_output);

    [[nodiscard]] double del_A_del_Z(size_t _layerIndex, size_t _j) const;
    [[nodiscard]] double del_C_del_A(size_t _layerIndex, size_t _k, std::vector<double> _expected) const;
    [[nodiscard]] double del_C_del_B(size_t _layerIndex, size_t _j) const;
    [[nodiscard]] double del_C_del_W(size_t _layerIndex, size_t _k, size_t _j) const;

    static double m_learning_rate;
};