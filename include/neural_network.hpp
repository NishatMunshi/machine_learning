#pragma once

#include "matrix.hpp"

class Neural_Network {
public:
    explicit Neural_Network(std::vector<size_t> const &_topology);

    std::vector<double> feed_forward(std::vector<double> const &_input);
    void backprop(std::vector<double> const &_expected);

    static double mean_squared_error(std::vector<double> const &_result, std::vector<double> const &_expected);

private:
    std::vector<Matrix> A;
    std::vector<Matrix> Z;
    std::vector<Matrix> W;
    std::vector<Matrix> B;

    // memoize the derivatives of cost function
    std::vector<Matrix> delC_delA;
    std::vector<Matrix> delA_delZ;
    std::vector<Matrix> delC_delW;
    std::vector<Matrix> delC_delB;

    static Matrix activation_function(Matrix const &_mat);
    static double activation_function(double _x);
    static double activation_function_derivative(double _x);

    void calculate_gradient(std::vector<double> const &_expected_output);

    [[nodiscard]] double del_A_del_Z(size_t _layerIndex, size_t _j) const;
    [[nodiscard]] double del_C_del_A(size_t _layerIndex, size_t _k, std::vector<double> _expected) const;
    [[nodiscard]] double del_C_del_B(size_t _layerIndex, size_t _j) const;
    [[nodiscard]] double del_C_del_W(size_t _layerIndex, size_t _k, size_t _j) const;

    static double m_learning_rate;
};