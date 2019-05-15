//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "gng_algo.h"
#include "../CImg.h"
#include "input_generator.h"
#include <functional>

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 640

using namespace cimg_library;

template <template <class> class Algorithm, class Trait>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

    vector<pair<int,int>> to_single_vec(vector< vector< pair<int,int> > > vec);
    void drawNode(int x, int y, CImg<unsigned char> &currentImg);
    void drawEdge(EdgePtr edge_ptr, NodePtr node_ptr, CImg<unsigned char> &currentImg);
    void drawCounter(CImg<unsigned char> &currentImg);
    void drawFigure(vector<pair<int, int>> positions, CImg<unsigned char> &currentImg);
    void drawPicture(vector< pair<int, int> >  positions, CImg<unsigned char> &currentImg);
    Algorithm<Trait> algo;
    
    CImgDisplay window;
    CImg<unsigned char> background;

    bool is_drawing = false;
    bool is_running = false;
public:
    GNGContainer() : algo(SCREEN_WIDTH, SCREEN_HEIGHT){
        background.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
        window.assign(background, "Growing Neural Gas");
    }
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

template< template <class> class Algorithm, class Trait>
class MovingPictureGNGContainer : public GNGContainer< Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;

    vector< pair<int, int>> pic_vector;
public:
    MovingPictureGNGContainer() : GNGContainer<Algorithm, Trait>(){}
    virtual void init();
    virtual void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
