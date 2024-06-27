#include "include/neural_network.hpp"
#include "E:\programming_tools\SFML\SFML_Sources\include\SFML\Graphics.hpp"
#include <iostream>

#define WINDOW_DIMENTION 900

int main() {
    std::vector<std::vector<double>> input = {{0, 0, 0},
                                              {0, 0, 1},
                                              {0, 1, 0},
                                              {0, 1, 1},
                                              {1, 0, 0},
                                              {1, 0, 1},
                                              {1, 1, 0},
                                              {1, 1, 1}};

    std::vector<std::vector<double>> expectation = {{0, 0},
                                                    {0, 1},
                                                    {0, 1},
                                                    {1, 0},
                                                    {0, 1},
                                                    {1, 0},
                                                    {1, 0},
                                                    {1, 1}};
    std::vector<size_t> const topology = {input.front().size(), 2, expectation.front().size()};
    Neural_Network network{topology};

    sf::RenderWindow window{sf::VideoMode(WINDOW_DIMENTION, WINDOW_DIMENTION), "machine_learning"};
    for (size_t line = 0; line < SIZE_MAX; ++line) {
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

        std::cout << err << '\n';

        auto const resolution = 100;

        if (err > 1e-3) {
            sf::Vertex point;
            point.position = {(float) line / resolution, WINDOW_DIMENTION - (float) err * WINDOW_DIMENTION};
            point.color = sf::Color::Red;
            window.draw(&point, 1, sf::Points);
        }
        window.display();

        if (line == WINDOW_DIMENTION * resolution) {
            line = 0;
        }
    }
}
