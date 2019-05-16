
#ifndef GRAPH_NULL_IMG_MANAGER_H
#define GRAPH_NULL_IMG_MANAGER_H

#include "../CImg.h"

using namespace cimg_library;

template <class Trait>
class ImgManager {
   
    CImgDisplay* window;
    CImg<unsigned char>* main_img;
    CImg<unsigned char>* drawing_img;

    public:
        ImgManager(CImgDisplaye* window);
        //Draws a single node at target coordinates
        void drawNode(int x, int y);

        //Draws a single edge from point x0,y0 to x1,y1
        void drawEdge(int x0, int y0, int x1, int y1);

        //Draws a number in the upper right corner
        void drawCounter(int num);

        //Used to draw all the graph
        void drawExtras(Graph<Trait> graph, int execution_time);

        //Draws point in space pixel by pixel
        void drawPicture(vector< pair<int,int> > vec);

        //Draws connected lines
        void drawLines(vector< pair<int,int> > vec);

        //Draws line (auxiliary to maintain abstraction)
        void drawLine(pair<int,int> p1, pair<int,int> p2, int color[3]);
};
#endif
