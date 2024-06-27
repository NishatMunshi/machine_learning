#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "include/neural_network.hpp"
#include "include/random_number_generator.hpp"
#include <stdexcept>
#include <cmath>
#include <iostream>

Neural_Network::Neural_Network(std::vector<size_t> const &_topology) {
    size_t const layers = _topology.size();
    if (layers < 2) {
        throw std::invalid_argument("cannot create neural network with less than two layers.");
    }

    Random_Number_Generator random{-1.0, 1.0};

    for (size_t layerIndex = 0; layerIndex < layers; ++layerIndex) {
        W.emplace_back(layerIndex == 0 ? 0 : _topology.at(layerIndex - 1), _topology.at(layerIndex));
        A.emplace_back(1, W.back().cols());
        Z.emplace_back(1, W.back().cols());
        B.emplace_back(1, W.back().cols());

        // randomize
        for (size_t j = 0; j < W.back().cols(); ++j) {
            B.back().at(0, j) = random.generate();
            for (size_t i = 0; i < W.back().rows(); ++i) {
                W.back().at(i, j) = random.generate();
            }
        }

    }
}

std::vector<double> Neural_Network::feed_forward(std::vector<double> const &_input) {
    if (_input.size() not_eq Z.front().cols()) {
        throw std::invalid_argument("input size not compatible with the current topology");
    }

    Z.front() = Matrix{_input}; // input layer
    A.front() = activation_function((Z[0])); // input layer

    for (size_t i = 1; i < W.size(); ++i) {
        Z[i] = (A[i - 1] * W[i]) + B[i];
        A[i] = activation_function(Z[i]);
    }

    return A.back().vector();
}

void Neural_Network::backprop(std::vector<double> const &_expected) {
    if (A.back().cols() not_eq _expected.size()) {
        throw std::invalid_argument("expected vector size not compatible with the current topology");
    }

    for (size_t layerIndex = A.size() - 1; true; layerIndex--) {
        auto const prevLayerLength = W.at(layerIndex).rows();
        auto const thisLayerLength = W.at(layerIndex).cols();

        for (size_t j = 0; j < thisLayerLength; ++j) {
            auto const db = del_C_del_B(layerIndex, j, _expected);
            B.at(layerIndex).at(0, j) -= db * eta;

            for (size_t k = 0; k < prevLayerLength; ++k) {
                auto const dw = del_C_del_W(layerIndex, k, db);
                W.at(layerIndex).at(k, j) -= dw * eta;
            }
        }

        if (layerIndex == 0) {
            break;
        }
    }
}

Matrix Neural_Network::activation_function(Matrix const &_mat) {
    Matrix Y = _mat;

    for (size_t i = 0; i < Y.rows(); ++i) {
        for (size_t j = 0; j < Y.cols(); ++j) {
            Y.at(i, j) = activation_function(_mat.at(i, j));
        }
    }

    return Y;
}

double Neural_Network::activation_function(double const _x) {
    return 1 / (1 + exp(-_x));
}

double Neural_Network::mean_squared_error(std::vector<double> const &_result, std::vector<double> const &_expected) {
    if (_result.size() not_eq _expected.size()) {
        throw std::invalid_argument("[MNE]: result and expected vectors have different sizes");
    }

    double error = 0.0;
    for (size_t i = 0; i < _result.size(); ++i) {
        double diff = _result.at(i) - _expected.at(i);
        error += diff * diff;
    }

    return error;
}

double Neural_Network::del_C_del_A(size_t _layerIndex, size_t _neuronIndex, std::vector<double> const &_expected) {
    if (_expected.size() not_eq A.back().cols()) {
        throw std::invalid_argument("[DELCDELA]: result and expected vectors have different sizes");
    }
    // base case
    if (_layerIndex == A.size() - 1) {
        return 2 * (A.back().at(0, _neuronIndex) - _expected.at(_neuronIndex));
    }

    double result = 0.0;
    size_t const nextLayerIndex = _layerIndex + 1;
    for (size_t j = 0; j < A.at(nextLayerIndex).cols(); ++j) {
        result += W.at(nextLayerIndex).at(_neuronIndex, j)
                  * activation_function_derivative(Z.at(nextLayerIndex).at(0, j))
                  * del_C_del_A(nextLayerIndex, j, _expected);
    }

    return result;
}

double Neural_Network::activation_function_derivative(double _x) {
    return exp(-_x) / ((1 + exp(-_x)) * (1 + exp(-_x)));
}

double Neural_Network::del_C_del_B(size_t _layerIndex, size_t _j, std::vector<double> const &_expected) {
    return activation_function_derivative(Z.at(_layerIndex).at(0, _j))
           * del_C_del_A(_layerIndex, _j, _expected);

}

double Neural_Network::del_C_del_W(size_t _layerIndex, size_t _k, double _del_c_del_b) {
    return A.at(_layerIndex - 1).at(0, _k)
           * _del_c_del_b;

}
double Neural_Network::eta = 1e-1;

#pragma clang diagnostic pop