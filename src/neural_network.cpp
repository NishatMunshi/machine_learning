#include "../include/neural_network.hpp"
#include "../include/random_number_generator.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

Neural_Network::Neural_Network(std::vector<std::size_t> const &_topology) {
    std::size_t const layers = _topology.size();
    if (layers < 2) {
        throw std::invalid_argument("cannot create neural network with less than two layers.");
    }

    Random_Number_Generator random{-1.0, 1.0};

    for (std::size_t layerIndex = 0; layerIndex < layers; ++layerIndex) {
        W.emplace_back(layerIndex == 0 ? 0 : _topology.at(layerIndex - 1), _topology.at(layerIndex));
        A.emplace_back(1, W.back().cols());
        Z.emplace_back(1, W.back().cols());
        B.emplace_back(1, W.back().cols());

        delC_delW.emplace_back(layerIndex == 0 ? 0 : _topology.at(layerIndex - 1), _topology.at(layerIndex));
        delC_delA.emplace_back(1, W.back().cols());
        delA_delZ.emplace_back(1, W.back().cols());
        delC_delB.emplace_back(1, W.back().cols());

        // randomize
        for (std::size_t j = 0; j < W.back().cols(); ++j) {
            B.back().at(0, j) = random.generate();
            for (std::size_t i = 0; i < W.back().rows(); ++i) {
                W.back().at(i, j) = random.generate();
            }
        }
    }
}

std::vector<double> Neural_Network::feed_forward(std::vector<double> const &_input) {
    if (_input.size() not_eq Z.front().cols()) {
        throw std::invalid_argument("input size not compatible with the current topology");
    }

    Z.front() = Matrix{_input};              // input layer
    A.front() = activation_function((Z[0])); // input layer

    for (std::size_t i = 1; i < W.size(); ++i) {
        Z[i] = (A[i - 1] * W[i]) + B[i];
        A[i] = activation_function(Z[i]);
    }

    return A.back().vector();
}

void Neural_Network::calculate_gradient(std::vector<double> const &_expected_output) {
    for (std::size_t layerIndex = A.size() - 1; true; layerIndex--) {
        auto const thisLayerLength = W.at(layerIndex).cols();
        auto const prevLayerLength = W.at(layerIndex).rows();

        for (std::size_t j = 0; j < thisLayerLength; ++j) {
            // order matters A->Z->B->W
            auto const da = del_C_del_A(layerIndex, j, _expected_output);
            delC_delA.at(layerIndex).at(0, j) = da;

            auto const dz = del_A_del_Z(layerIndex, j);
            delA_delZ.at(layerIndex).at(0, j) = dz;

            auto const db = del_C_del_B(layerIndex, j);
            delC_delB.at(layerIndex).at(0, j) = db;

            for (std::size_t k = 0; k < prevLayerLength; ++k) {
                auto const dw = del_C_del_W(layerIndex, k, j);
                delC_delW.at(layerIndex).at(k, j) = dw;
            }
        }

        if (layerIndex == 0) {
            break;
        }
    }
}

double Neural_Network::del_C_del_W(size_t _layerIndex, size_t _k, size_t _j) const {
    return A.at(_layerIndex - 1).at(0, _k)        // how we affect z
           * delC_delB.at(_layerIndex).at(0, _j); // how z affects a and how a affects cost;
}

double Neural_Network::del_C_del_B(size_t _layerIndex, size_t _j) const {
    return 1                                      // how we affect z
           * delA_delZ.at(_layerIndex).at(0, _j)  // how z affects a
           * delC_delA.at(_layerIndex).at(0, _j); // how a affects cost
}

double Neural_Network::del_C_del_A(std::size_t _layerIndex, std::size_t _k, std::vector<double> _expected) const {
    // base case
    if (_layerIndex == A.size() - 1) {
        return 2 * (A.back().at(0, _k) - _expected.at(_k));
    }

    double result = 0.0;
    std::size_t const nextLayerIndex = _layerIndex + 1;
    for (std::size_t j = 0; j < A.at(nextLayerIndex).cols(); ++j) {
        result += W.at(nextLayerIndex).at(_k, j)           // how we affect z of next layer
                  * delA_delZ.at(nextLayerIndex).at(0, j)  // how z of next layer affects a of next layer
                  * delC_delA.at(nextLayerIndex).at(0, j); // how a of next layer affects cost
    }
    return result;
}

double Neural_Network::del_A_del_Z(size_t _layerIndex, size_t _j) const {
    return activation_function_derivative(Z.at(_layerIndex).at(0, _j));
}

void Neural_Network::backprop(std::vector<double> const &_expected) {
    if (A.back().cols() not_eq _expected.size()) {
        throw std::invalid_argument("expected vector size not compatible with the current topology");
    }

    calculate_gradient(_expected);

    // update the weights and the biases
    for (std::size_t i = A.size() - 1; true; i--) {
        auto x = (delC_delW.at(i) * m_learning_rate);
        W.at(i) = W.at(i) - (delC_delW.at(i) * m_learning_rate);
        B.at(i) = B.at(i) - (delC_delB.at(i) * m_learning_rate);
        if (i == 0) {
            break;
        }
    }
}

Matrix Neural_Network::activation_function(Matrix const &_mat) {
    Matrix Y = _mat;

    for (std::size_t i = 0; i < Y.rows(); ++i) {
        for (std::size_t j = 0; j < Y.cols(); ++j) {
            Y.at(i, j) = activation_function(_mat.at(i, j));
        }
    }

    return Y;
}

double Neural_Network::activation_function(double const _x) {
    return 1 / (1 + exp(-_x));
}

double Neural_Network::activation_function_derivative(double _x) {
    return exp(-_x) / ((1 + exp(-_x)) * (1 + exp(-_x)));
}

double Neural_Network::mean_squared_error(std::vector<double> const &_result, std::vector<double> const &_expected) {
    if (_result.size() not_eq _expected.size()) {
        throw std::invalid_argument("[MNE]: result and expected vectors have different sizes");
    }

    double error = 0.0;
    for (std::size_t i = 0; i < _result.size(); ++i) {
        double diff = _result.at(i) - _expected.at(i);
        error += diff * diff;
    }

    return error;
}

double Neural_Network::m_learning_rate = 1e-1;