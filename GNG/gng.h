//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "gng_exec.h"
#include "gng_algo.h"
#include <SFML/Graphics.hpp>
#include <functional>

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 640

using namespace cimg_library;

template <template <class> class Algorithm, class Trait>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

    sf::VertexArray to_single_vec(sf::PrimitiveType primitive, vector< sf::VertexArray > vec); 
    void drawExtras(Graph<Trait> graph, int exec_count);
    
    void imageWait(int time);
    void drawPicture(sf::VertexArray);
    void getInput(GNGExec<Algorithm, Trait> &exe);
    Algorithm<Trait>* algo;
    
    sf::RenderWindow window;

    bool is_drawing = false;
public:
    GNGContainer() {
        algo = new Algorithm<Trait>(SCREEN_WIDTH, SCREEN_HEIGHT);
        window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Growing Neural Gas");
    }
    virtual void init();
    virtual void start();
};

template<template <class> class Algorithm, class Trait>
class PictureGNGContainer : public GNGContainer<Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;

    sf::VertexArray pic_vertices; 
public:
    PictureGNGContainer() : GNGContainer<Algorithm, Trait>(){pic_vertices.setPrimitiveType(sf::Points);}
    virtual void init();
    virtual void start();
};

template< template <class> class Algorithm, class Trait>
class MovingPictureGNGContainer : public GNGContainer< Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;

    sf::VertexArray pic_vertices;
public:
    MovingPictureGNGContainer() : GNGContainer<Algorithm, Trait>(){this->pic_vertices.setPrimitiveType(sf::Points);}
    virtual void init();
    virtual void start();
};
template< template <class> class Algorithm, class Trait>
class VideoGNGContainer : public GNGContainer<Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;
   
public:
    VideoGNGContainer() : GNGContainer<Algorithm, Trait>(){}
    virtual void init();
    virtual void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
