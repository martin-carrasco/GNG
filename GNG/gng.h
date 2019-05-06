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

template <class Trait, class Algorithm>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Node<Graph<Trait>>* EdgePtr;

    void drawNode(int x, int y);
    void drawEdge(EdgePtr edge_ptr, NodePtr node_ptr);
    void drawCounter();

    Algorithm<Trait> algo;
    
    sf::RenderWindow window;
    bool is_drawing = false;
    bool is_running = false;
public:
    GNGContainer() : algo(SCREEN_HEIGHT, SCREEN_WIDTH) {}
    virtual void init() = 0;
    virtual void start() = 0;
};

template<class Trait, class Algorithm>
class DefaultGNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
public:
    DefaultGNGContainer() :  GNGContainer<Trait, Algorithm>() {}
    virtual void init();
    virtual void start();
};

template<class Trait, class Algorithm>
class PictureGNGContainer{
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

    vector<pair<int, int>> pic_vector; 
public:
    PictureGNGContainer() : GNGContainer<Trait, Algorithm>(){}
    virtual void init();
    virtual void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
