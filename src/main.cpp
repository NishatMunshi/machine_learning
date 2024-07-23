#include "../include/neural_network.hpp"
#include "SFML/Graphics.hpp"

#include <cmath>
#include <iostream>

#define WINDOW_DIMENTION 900

int main() {
    // training set generation
    std::vector<std::vector<double>> input{{0, 0},
                                           {0, 1},
                                           {1, 0},
                                           {1, 1}};
    std::vector<std::vector<double>> expectation{{0},
                                                 {1},
                                                 {1},
                                                 {0}};
    // -------------------------------------

    std::vector<size_t> const topology = {input.front().size(), 2, expectation.front().size()};
    Neural_Network network{topology};

    sf::RenderWindow window{sf::VideoMode(WINDOW_DIMENTION, WINDOW_DIMENTION), "machine_learning"};
    for (size_t epoch = 0; epoch < SIZE_MAX; ++epoch) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                return 0;
            }
        }

        sf::Vertex point;
        double error = 0.0;
        for (size_t j = 0; j < input.size(); ++j) {
            auto output = network.feed_forward(input[j]);
            network.backprop(expectation[j]);
            error += Neural_Network::mean_squared_error(output, expectation[j]);
        }

        point.position.x = 5 * epoch / WINDOW_DIMENTION;
        point.position.y = WINDOW_DIMENTION - WINDOW_DIMENTION * error;
        point.color = sf::Color::Red;
        window.draw(&point, 1, sf::Points);
        window.display();
    }
}
