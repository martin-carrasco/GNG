
#ifndef GRAPH_NULL_IMG_MANAGER_H
#define GRAPH_NULL_IMG_MANAGER_H

#include "../CImg.h"
#include "../graph.h"

using namespace cimg_library;

template <class Trait>
class ImgManager { 
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;

    CImgDisplay* window;
    CImg<unsigned char>* main_img;

    public:
        ~ImgManager(){
            delete main_img;
        }
        ImgManager(CImgDisplay* window){
            this->window = window;
            main_img = new CImg<unsigned char>(this->window->width(), this->window->height(), 1, 3, 0);
        }
        ImgManager(CImgDisplay* window, CImg<unsigned char>* img){
            this->window = window;
            this->main_img = img;
        }

        //Clears the main image
        void clearMain();

        //Draws a single node at target coordinates
        void drawNode(int x, int y, CImg<unsigned char> &img);

        //Draws a single edge from point x0,y0 to x1,y1
        void drawEdge(int x0, int y0, int x1, int y1, CImg<unsigned char> &img);

        //Draws a number in the upper right corner
        void drawCounter(int num, CImg<unsigned char> &img);

        //Used to draw all the graph
        void drawExtras(vector<NodePtr> nodes, int execution_time);

        //Draws point in space pixel by pixel
        void drawPicture(vector< pair<int,int> > vec);

        //Draws connected lines
        void drawLines(vector< pair<int,int> > vec);

        //Draws line (auxiliary to maintain abstraction)
        void drawLine(pair<int,int> p1, pair<int,int> p2, const int* color);
        
        //Changes the image beign pointed to
        void changeImage(CImg<unsigned char>* img);
};
#include "img_manager.cpp"
#endif
