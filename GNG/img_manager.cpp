#include "img_manager.h"
#include "../CImg.h"



static constexpr int RED[] = {255, 0, 0};
static constexpr int GREEN[] = {0, 255, 0};
static constexpr int WHITE[] = {255, 255, 255};
static constexpr int BLUE[] = {0, 0, 255};

template< class Trait >
void ImgManager<Trait>::changeImage(CImg<unsigned char>* img){
    this->main_img = img;
}
template< class Trait >
void ImgManager<Trait>::clearMain(){
    this->main_img->fill(0);
}

template< class Trait >
void ImgManager<Trait>::drawLine(pair<int,int> p1, pair<int,int> p2, const int* color){
    main_img->draw_line(p1.first, p1.second, p2.first, p2.second, color);
    main_img->display(*(this->window));
}

template<class Trait>
void ImgManager<Trait>::drawPicture(vector< pair<int, int> >  positions){
    for(auto current_point : positions){
            main_img->draw_point(current_point.first, current_point.second, WHITE);
    }
    main_img->display(*(this->window));
}

template< class Trait >
void ImgManager<Trait>::drawLines(vector<pair<int, int>> positions){ 
    pair<int, int> last_point = make_pair(-1, -1);
    for(auto current_point : positions){
        if(last_point.first == -1 && last_point.second == -1){
            last_point = current_point;
            continue;
        }
        main_img->draw_line(last_point.first, last_point.second, current_point.first, current_point.second, WHITE);
        last_point = current_point;
    }
}
template< class Trait >
void ImgManager<Trait>::drawNode(int x, int y, CImg<unsigned char> &img){

    img.draw_circle(x,y, 5, GREEN);
}
template< class Trait >
void ImgManager<Trait>::drawEdge(int x0, int y0, int x1, int y1, CImg<unsigned char> &img){
    img.draw_line(x0, y0, x1, y1, RED);
}

template< class Trait >
void ImgManager<Trait>::drawCounter(int number, CImg<unsigned char> &img){
    string iter_string = to_string(number);
    img.draw_text(this->window->width() - 50, 20, iter_string.c_str(), BLUE);
}
template<class Trait>
void ImgManager<Trait>::drawExtras(vector<NodePtr> nodes, int exec_count){

    CImg<unsigned char>* current_img = new CImg<unsigned char>(this->window->width(), this->window->height(), 1, 3, 0);
    *current_img = *this->main_img; 

    int x0, y0, x1, y1;

    drawCounter(exec_count, *current_img);
    for(NodePtr node_ptr : nodes){
        
        x0 = node_ptr->getContent().pos[0];
        y0 = node_ptr->getContent().pos[1];

        this->drawNode(x0, y0, *current_img);

        for(EdgePtr edge_ptr : node_ptr->getEdges()){
            //if(DEBUG){
                NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
                x1 = node_dest_ptr->getContent().pos[0];
                y1 = node_dest_ptr->getContent().pos[1];
                
                this->drawEdge(x0, y0, x1, y1, *current_img);
            //} //TODO FIGURE IT OUT!
        }
    }
    (*current_img).display(*(this->window));
    delete current_img;
}

