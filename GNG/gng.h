//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "gng_exec.h"
#include "img_manager.h"
#include "../CImg.h"
#include "gng_algo.h"
#include <functional>

#define SCREEN_HEIGHT 640
#define SCREEN_WIDTH 640

using namespace cimg_library;

template <class Trait>
class GNGContainer {
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

    vector<pair<int,int>> to_single_vec(vector< vector< pair<int,int> > > vec); 

    CImgDisplay* window;

    bool is_drawing = false;
public:
    ~GNGContainer(){
        delete window;
    }
    GNGContainer() {
        window = new CImgDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, "Growing Neural Gas");
    }

    virtual void init();
    virtual void start();
};

template< class Trait>
class PictureGNGContainer : public GNGContainer<Trait> {
    friend class GNGContainer<Trait>;
    typedef typename GNGContainer<Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Trait>::EdgePtr EdgePtr;

    vector<pair<int, int>> pic_vector; 
public:
    PictureGNGContainer() : GNGContainer<Trait>(){}
    virtual void init();
    virtual void start();

};

template<class Trait>
class MovingPictureGNGContainer : public GNGContainer<Trait> {
    friend class GNGContainer<Trait>;
    typedef typename GNGContainer<Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Trait>::EdgePtr EdgePtr;

    void movePositions(int x, int y);
    vector< pair<int, int>> pic_vector;
public:
    MovingPictureGNGContainer() : GNGContainer<Trait>(){}
    virtual void init();
    virtual void start();
    
};
template<class Trait>
class VideoGNGContainer : public GNGContainer<Trait> {
    friend class GNGContainer<Trait>;
    typedef typename GNGContainer<Trait>::NodePtr NodePtr;
    typedef typename GNGContainer<Trait>::EdgePtr EdgePtr;
   
    CImg<unsigned char>* current_frame;
    CImgList<unsigned char> frame_list;
public:
    VideoGNGContainer() : GNGContainer<Trait>(){}
    virtual void init();
    virtual void start();

    vector< pair<int,int> > parseFrame(CImg<unsigned char> frame);
    vector<pair<int,int>> getBinaryPoints(CImg<unsigned char> img);
    void binarizeImg(CImg<unsigned char> &img);
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
