#include "include/neural_network.hpp"
#include "E:\programming_tools\SFML\SFML_Sources\include\SFML\Graphics.hpp"
#include <iostream>

#define WINDOW_DIMENTION 900

int main() {
    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> expectation;
#define BITS 4
#define TWO_POWER(X) (1ull <<(X))
    for (size_t i = 0; i < TWO_POWER(BITS); ++i) {
        std::vector<double> i_bits(BITS);
        for (size_t k = 0; k < BITS; ++k) {
            i_bits.at(BITS - k - 1) = double((i >> k) & 1);
        }
        input.push_back(i_bits);

        auto i_l = i & (TWO_POWER(BITS / 2) - 1);
        auto i_h = i >> (BITS / 2);
        auto sum = i_l + i_h;

        std::vector<double> s_bits(BITS / 2 + 1);
        for (size_t k = 0; k < BITS / 2 + 1; ++k) {
            s_bits.at(BITS / 2 - k) = double((sum >> k) & 1);
        }
        expectation.push_back(s_bits);
    }

    std::vector<size_t> const topology = {input.front().size(), BITS, expectation.front().size()};
    Neural_Network network{topology};

    sf::RenderWindow window{sf::VideoMode(WINDOW_DIMENTION, WINDOW_DIMENTION), "machine_learning"};
    for (size_t epoch = 0; epoch < SIZE_MAX; ++epoch) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        double err = 0.0;
        for (size_t j = 0; j < input.size(); ++j) {
            auto output = network.feed_forward(input[j]);
            err += Neural_Network::mean_squared_error(output, expectation[j]);
            network.backprop(expectation[j]);
        }

        std::cout << epoch << ": " << err << '\n';

        auto const resolution = 50;

        if (window.hasFocus()) {
            if (err > 1e-3) {
                sf::Vertex point;
                point.position = {(float) epoch / resolution, WINDOW_DIMENTION - (float) err * WINDOW_DIMENTION / float(
                        TWO_POWER(BITS)) / 1.5f};
                point.color = sf::Color::Red;
                window.draw(&point, 1, sf::Points);
            }
            window.display();
        }

        if (epoch == WINDOW_DIMENTION * resolution) {
            epoch = 0;
        }
    }
}
