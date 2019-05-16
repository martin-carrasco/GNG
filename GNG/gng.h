//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "gng_exec.h"
#include "gng_algo.h"
#include "../CImg.h"
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
    void drawExtras(Graph<Trait> graph, int exec_count, CImg<unsigned char> &current_img);
    
    void drawFigure(vector<pair<int, int>> positions, CImg<unsigned char> &current_img);
    void getInput(GNGExec<Algorithm, Trait> &exe);
    Algorithm<Trait>* algo;
    
    CImgDisplay window;
    CImg<unsigned char> background;

    bool is_drawing = false;
public:
    GNGContainer() {
        algo = new Algorithm<Trait>(SCREEN_WIDTH, SCREEN_HEIGHT);
        background.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
        window.assign(background, "Growing Neural Gas");
    }
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
template< template <class> class Algorithm, class Trait>
class VideoGNGContainer : public GNGContainer<Algorithm, Trait> {
    friend class GNGContainer<Algorithm, Trait>;
    typedef typename GNGContainer<Algorithm, Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Algorithm, Trait>::EdgePtr EdgePtr;
   
    CImg<unsigned char>* current_frame;
    CImgList<unsigned char> frame_list;
public:
    VideoGNGContainer() : GNGContainer<Algorithm, Trait>(){}
    virtual void init();
    virtual void start();

    vector<pair<int,int>> getBinaryPoints(CImg<unsigned char> img);
    void binarizeImg(CImg<unsigned char> &img);
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
