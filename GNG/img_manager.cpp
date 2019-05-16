#include "img_manager.h"
#include "../CImg.h"



template< class Trait > 
ImgManager<Trait>::ImgManager<Trait>(CImgDisplay* window) {
        this->window = window;
        //TODO get w and h from window
        drawing_img.assign(this->window.width(), height(), 1, 3, 0);
        main_img.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
}
template<class Trait>
void ImgManager<Trait>::drawPicture(vector< pair<int, int> >  positions){
    for(auto current_point : positions){
            drawing_img.draw_point(current_point.first, current_point.second, WHITE);
    }
}

template< class Trait >
void ImgManager<Trait>::drawLines(vector<pair<int, int>> positions){ 
    pair<int, int> last_point = make_pair(-1, -1);
    for(auto current_point : positions){
        if(last_point.first == -1 && last_point.second == -1){
            last_point = current_point;
            continue;
        }
        drawing_img.draw_line(last_point.first, last_point.second, current_point.first, current_point.second, WHITE);
        last_point = current_point;
    }
}
template< class Trait >
void ImgManager<Trait>::drawNode(int x, int y){

    this->drawing_img.draw_circle(node_ptr->getContent().pos[0], 
                            node_ptr->getContent().pos[1], 
                            5, 
                            GREEN);
}
template< class Trait >
void ImgManager<Trait>::drawEdge(int x0, int y0, int x1, int y1){
    current_img.draw_line(x0, y0, x1, y1, RED);
}

template< class Trait >
void ImgManager<Trait>::drawCounter(int number){
    string iter_string = to_string(exec_count);
    drawing_img.draw_text(SCREEN_WIDTH - 50, 20, iter_string.c_str(), BLUE);
}
//TODO Has to draw to window
//TODO figure out where to put it
template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawExtras(Graph<Trait> graph, int exec_count, CImg<unsigned char> &current_img){


    for(NodePtr node_ptr : graph.getNodesVector()){

        drawNode(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);

        for(EdgePtr edge_ptr : node_ptr->getEdges()){

	        NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr); 
            if(DEBUG){
            }
        }
    }
}
