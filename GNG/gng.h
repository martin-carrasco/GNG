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

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 640


template<class Trait>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    GNGAlgorithm<Trait> algo;
    //Ventana del GNG
    sf::RenderWindow window;

    //Vector de posicion de lineas auxiliar
    std::vector<sf::Vector2f> lineas;

    //Auxiliar para dibujo
    bool is_drawing = false;

    //Variable de pause
    bool is_running = false;
public:
    GNGContainer() :  algo(SCREEN_HEIGHT, SCREEN_WIDTH) {}
    virtual void init();
    virtual void start();
};
template<class Trait>
class PictureGNGContainer : public GNGContainer<Trait> {
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    vector<pair<int, int>> pic_vector; 
public:
    PictureGNGContainer() = default;
    virtual void init();
    virtual void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
