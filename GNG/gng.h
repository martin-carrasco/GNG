//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "gng_algo.h"
#include "input_generator.h"
#include "SFML/Graphics.hpp"
#include <random>
#include <functional>


template<class GNGTrait>
class GNGContainer{
    typedef ::Node<Graph<GNGTrait>>* NodePtr;
    typedef ::Edge<Graph<GNGTrait>>* EdgePtr;
    GNGAlgorithm<GNGTrait> algo;
    //Ventana del GNG
    sf::RenderWindow window;

    //Vector de posicion de lineas auxiliar
    std::vector<sf::Vector2f> lineas;

    //Auxiliar para dibujo
    bool is_drawing = false;

    //Variable de pause
    bool is_running = false;
public:
    GNGContainer() = default;
    void init();
    void start();
    void start_moving();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
