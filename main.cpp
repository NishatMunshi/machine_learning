#include "E:\programming_tools\SFML\SFML_Sources\include\SFML\Graphics.hpp"
#include "include/neural_network.hpp"
#include <cmath>
#include <iostream>

#define WINDOW_DIMENTION 900

int main() {
    // training set generation
    std::vector<double> s1 = {0, 1, 1, 2, 3, 4, 7, 11, 18, 29};
    std::vector<std::vector<double>> input{};
    std::vector<std::vector<double>> expectation{};
    for (size_t i = 0; i < s1.size() - 2; ++i) {
        input.emplace_back(std::vector<double>{s1.at(i) / 30, s1.at(i + 1) / 30});
        expectation.emplace_back(std::vector<double>{s1.at(i + 2) / 30});
    }
    // -------------------------------------

    std::vector<size_t> const topology = {input.front().size(), expectation.front().size()};
    Neural_Network network{topology};

    // sf::RenderWindow window{sf::VideoMode(WINDOW_DIMENTION, WINDOW_DIMENTION), "machine_learning"};
    for (size_t epoch = 0; epoch < SIZE_MAX; ++epoch) {
        //        sf::Event event{};
        //        while (window.pollEvent(event))
        //        {
        //            if (event.type == sf::Event::Closed)
        //            {
        //                window.close();
        //            }
        //        }

        //        window.clear();
        //        sf::Vertex point;
        double error = 0.0;
        for (size_t j = 0; j < input.size(); ++j) {
            //            point.position.x = float(input[j].back() / RANGE + 1) * WINDOW_DIMENTION / 2;
            //            point.position.y = float(expectation[j].back() - 0.5) * 300 + 450;
            //            point.color = sf::Color::Green;
            //            window.draw(&point, 1, sf::Points);

            auto output = network.feed_forward(input[j]);

            //            point.position.y = float(output.back() - 0.5) * 300 + 450;
            //            point.color = sf::Color::Yellow;
            //            window.draw(&point, 1, sf::Points);

            network.backprop(expectation[j]);
            error += Neural_Network::mean_squared_error(output, expectation[j]);
        }

        std::cout << error << std::endl;
        if (error < 0.01) {
            auto out = network.feed_forward({18.0 / 30, 29.0 / 30});
            std::cout << "f(29) = " << out.front() * 30 << std::endl;
        }
        //        window.display();
    }
}
