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

template <template <class> class Algorithm, class Trait>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

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

template<template <class> class Algorithm, class Trait>
class DefaultGNGContainer : public GNGContainer<Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;
public:
    DefaultGNGContainer() :  GNGContainer<Algorithm, Trait>() {}
    virtual void init();
    virtual void start();
};

template<template <class> class Algorithm, class Trait>
class PictureGNGContainer : public GNGContainer<Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;

    vector<pair<int, int>> pic_vector; 
public:
    PictureGNGContainer() : GNGContainer<Algorithm, Trait>(){}
    virtual void init();
    virtual void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
