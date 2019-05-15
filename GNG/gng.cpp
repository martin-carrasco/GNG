#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"
#include <sstream>
#include "../CImg.h"

#define DEBUG 1

using namespace cimg_library;

static constexpr int RED[] = {255, 0, 0};
static constexpr int GREEN[] = {0, 255, 0};
static constexpr int WHITE[] = {255, 255, 255};
static constexpr int BLUE[] = {0, 0, 255};



template< template <class> class Algorithm, class Trait>
vector<pair<int,int>> GNGContainer<Algorithm, Trait>::to_single_vec(vector< vector< pair<int, int> > > vec){
    vector<pair<int, int>> toReturn;
    for(auto current_vector : vec){
        for(auto current_pair : current_vector){
            toReturn.push_back(current_pair);
        }
    }
    return toReturn;
}
template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawPicture(vector< pair<int, int> >  positions, CImg<unsigned char> &currentImg){
    for(auto current_point : positions){
            currentImg.draw_point(current_point.first, current_point.second, WHITE);
    }
}

template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawFigure(vector<pair<int, int>> positions, CImg<unsigned char> &currentImg){ 
    pair<int, int> last_point = make_pair(-1, -1);
    for(auto current_point : positions){
        if(last_point.first == -1 && last_point.second == -1){
            last_point = current_point;
            continue;
        }
        if(DEBUG)
            cout << "Drawing point at x0: " << current_point.first << " y0: " << current_point.second
            << " x1: " << last_point.first << " y:1 " << last_point.second << endl;
        currentImg.draw_line(last_point.first, last_point.second, current_point.first, current_point.second, WHITE);
        last_point = current_point;
    }
}

template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawNode(int x, int y, CImg<unsigned char> &currentImg){
    currentImg.draw_circle(x, y, 5, GREEN);
}
template <template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawCounter(CImg<unsigned char> &currentImg){
    string iter_string = to_string(algo.get_iteracion());
    currentImg.draw_text(SCREEN_WIDTH - 50, 20, iter_string.c_str(), BLUE);
}
template <template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawEdge(EdgePtr edge_ptr, NodePtr node_ptr, CImg<unsigned char> &currentImg){
	NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
    
    if(DEBUG){
        int x0,y0,x1,y1;
        x0 = node_ptr->getContent().pos[0];
        y0 = node_ptr->getContent().pos[1];
        
        x1 = node_dest_ptr->getContent().pos[0];
        y1 = node_dest_ptr->getContent().pos[1];

        currentImg.draw_line(x0, y0, x1, y1, RED);
    }
}

template<template <class> class Algorithm, class Trait>
void DefaultGNGContainer<Algorithm, Trait>::init(){
	this->algo.init();
}

template<template <class> class Algorithm, class Trait>
void DefaultGNGContainer<Algorithm, Trait>::start() {
	bool is_pressed = false;
    vector< vector< pair<int, int> > > drawing_points;
    CImg<unsigned char> currentImg(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
    while (!this->window.is_closed()) {

        this->drawFigure(this->to_single_vec(drawing_points), currentImg);	
        if (this->is_running) {
            vector<pair<int,int>> vector_points = this->to_single_vec(drawing_points);
            cout << "Size de puntos: " << drawing_points.size() << endl;
			UniformDistributionInputGenerator<Trait>inpt_gen = UniformDistributionInputGenerator<Trait>(vector_points);
            cout << "Entro al loop con size: " << inpt_gen.size() << endl;
            while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 40000){
                CImg<unsigned char> point_img(currentImg);

				pair<int, int> tmp = inpt_gen.pop();
				this->algo.exec(tmp);

				Graph<Trait> g = this->algo.get_graph();

				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
				   this->drawNode(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1], point_img);	

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
				        this->drawEdge(edge_ptr, node_ptr, point_img);	
					}
				}
                this->drawCounter(point_img); 
                point_img.display(this->window);
                cimg::wait(1);
			}
            if(inpt_gen.size()  <= 0)
                cout << "Input run out" << endl;
            this->is_running = false;

		}

        if(this->window.is_key() && this->window.is_keyESC()){
            drawing_points.clear();
            currentImg.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
            cout << "cleared " << endl;
        }
        if(this->window.is_key() && this->window.is_keySPACE()){
            this->is_running = !this->is_running;
            cout << "Entro a space" << endl;
        }
        if(this->window.button()){
            vector<pair<int, int>> tmp_cord_holder;
            while( this->window.button() & 1){ 
                pair<int, int> click_pair = make_pair(this->window.mouse_x(), this->window.mouse_y());         
                tmp_cord_holder.push_back(click_pair);
                
                //Print to screen whats currently beign drawn
                if(tmp_cord_holder.size() > 1){
                    pair<int, int> last_pos = make_pair(-1, -1);
                    for(auto p : tmp_cord_holder){
                        if(last_pos.first == -1 && last_pos.second == -1){
                            last_pos.first = p.first;
                            last_pos.second = p.second;
                            continue;
                        }
                        currentImg.draw_line(p.first, p.second, last_pos.first, last_pos.second, WHITE);
                        last_pos = p;        
                    }
                }

                currentImg.display(this->window);
            }
            if(tmp_cord_holder.size() != 0)
                drawing_points.push_back(tmp_cord_holder);
        }
        currentImg.display(this->window);
        this->window.wait();
	}
}
template <template <class> class Algorithm, class Trait>
void PictureGNGContainer<Algorithm, Trait>::init(){ 
	this->algo.init();
    string line;
    ifstream file("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/output.txt");
    while(getline(file, line)){
        vector<string> tokens;
        stringstream tmp_read(line);
        string tmp_holder;
        while(getline(tmp_read, tmp_holder,',')){
            tokens.push_back(tmp_holder);
        }
        pair<int, int> tmp_pair = make_pair(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
        //cout << tmp_pair.first << ", " << tmp_pair.second << endl; 
        this->pic_vector.push_back(tmp_pair);
    }
    file.close();
} 

template <template <class> class Algorithm, class Trait>
void PictureGNGContainer<Algorithm, Trait>::start(){
    vector<vector<pair<int, int>>> drawing_points;
    drawing_points.push_back(this->pic_vector);
    CImg<unsigned char> currentImg(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
    while (!this->window.is_closed()) {

        this->drawPicture(this->to_single_vec(drawing_points), currentImg);	
        if (this->is_running) {
            vector<pair<int,int>> vector_points = this->to_single_vec(drawing_points);
            cout << "Size de puntos: " << vector_points.size() << endl;
			UniformDistributionInputGenerator<Trait>inpt_gen = UniformDistributionInputGenerator<Trait>(vector_points);
            cout << "Entro al loop con size: " << inpt_gen.size() << endl;
            while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 100000){
                //cout << "Max Age: " << (this->algo).getMaxAge() << endl;
                //cout << "Max Mean Error: " << (this->algo).findMaxMeanError() << endl; 
                CImg<unsigned char> point_img(currentImg);

				pair<int, int> tmp = inpt_gen.pop();
				this->algo.exec(tmp);

				Graph<Trait> g = this->algo.get_graph();

				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
				   this->drawNode(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1], point_img);	

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
				        this->drawEdge(edge_ptr, node_ptr, point_img);	
					}
				}
                this->drawCounter(point_img); 
                point_img.display(this->window);
               // cimg::wait(1);
			}
            if(inpt_gen.size()  <= 0)
                cout << "Input run out" << endl;
            this->is_running = false;

		}
        if(this->window.is_key() && this->window.is_keySPACE()){
            this->is_running = !this->is_running;
            cout << "Entro a space" << endl;
        }
        currentImg.display(this->window);
        this->window.wait();
	}
}

template <template <class> class Algorithm, class Trait>
void MovingPictureGNGContainer<Algorithm, Trait>::init(){ 
	this->algo.init();
    string line;
    ifstream file("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/output.txt");
    while(getline(file, line)){
        vector<string> tokens;
        stringstream tmp_read(line);
        string tmp_holder;
        while(getline(tmp_read, tmp_holder,',')){
            tokens.push_back(tmp_holder);
        }
        pair<int, int> tmp_pair = make_pair(atoi(tokens[0].c_str()), atoi(tokens[1].c_str()));
        //cout << tmp_pair.first << ", " << tmp_pair.second << endl; 
        this->pic_vector.push_back(tmp_pair);
    }
    file.close();
} 
template <template <class> class Algorithm, class Trait>
void MovingPictureGNGContainer<Algorithm, Trait>::start(){
    vector<pair<int, int>> drawing_points; 
    drawing_points = this->pic_vector;
    while (!this->window.is_closed()) {
        if (this->is_running) {
			MovingUniformDistributionInputGenerator<Trait>inpt_gen = MovingUniformDistributionInputGenerator<Trait>(drawing_points);
            while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 100000){
                CImg<unsigned char> currentImg(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
                
                drawing_points = inpt_gen.getInput();

                this->drawPicture(drawing_points, currentImg);

                //cout << "Max Age: " << (this->algo).getMaxAge() << endl;
                //cout << "Max Mean iError: " << (this->algo).findMaxMeanError() << endl; 
                
				pair<int, int> tmp = inpt_gen.pop();
				this->algo.exec(tmp);

				Graph<Trait> g = this->algo.get_graph();

				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
				   this->drawNode(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1],currentImg);	

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
				        this->drawEdge(edge_ptr, node_ptr, currentImg);	
					}
				}
                this->drawCounter(currentImg); 
                currentImg.display(this->window);
               // cimg::wait(1);
			}
            if(inpt_gen.size()  <= 0)
                cout << "Input run out" << endl;
            this->is_running = false;

		}
        if(this->window.is_key() && this->window.is_keySPACE()){
            this->is_running = !this->is_running;
            cout << "Entro a space" << endl;
        }
        this->window.wait();
	}
}
