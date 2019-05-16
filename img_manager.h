#include "../CImg.h"

using namespace cimg_library;

template <Trait>
class ImageManager {
   
    CImg
    public:
        //Draws a single node at target coordinates
        void drawNode(int x, int y);
        void drawEdge(int x0, int y0, int x1, int y1);
        void drawCounter(int num);

        void drawExtras(Graph<Trait> graph, int execution_time);

        void drawPicture();
};
