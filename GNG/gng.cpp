#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"
#include <sstream>
#include "../CImg.h"

#define DEBUG 1
#define MAX_EXECUTIONS 5000

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
<<<<<<< HEAD
void GNGContainer<Algorithm, Trait>::getInput(GNGExec<Algorithm, Trait> &exe){

    if(this->window.is_key() && this->window.is_keySPACE()){
        exe.setRunning(true);
=======
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
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
    }
    this->window.wait();
}
<<<<<<< HEAD
template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawFigure(vector< pair<int, int> >  positions, CImg<unsigned char> &current_img){
    for(auto current_point : positions){
            current_img.draw_point(current_point.first, current_point.second, WHITE);
    }
=======

template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawNode(int x, int y, CImg<unsigned char> &currentImg){
    currentImg.draw_circle(x, y, 5, GREEN);
}
template <template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawCounter(CImg<unsigned char> &currentImg){
    string iter_string = to_string(algo.get_iteracion());
    currentImg.draw_text(SCREEN_WIDTH - 50, 20, iter_string.c_str(), BLUE);
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
}
template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawExtras(Graph<Trait> graph, int exec_count, CImg<unsigned char> &current_img){

    string iter_string = to_string(exec_count);
    current_img.draw_text(SCREEN_WIDTH - 50, 20, iter_string.c_str(), BLUE);

    for(NodePtr node_ptr : graph.getNodesVector()){

        current_img.draw_circle(node_ptr->getContent().pos[0], 
                                node_ptr->getContent().pos[1], 
                                5, 
                                GREEN);
        for(EdgePtr edge_ptr : node_ptr->getEdges()){

	        NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr); 
            if(DEBUG){
                int x0,y0,x1,y1;
                x0 = node_ptr->getContent().pos[0];
                y0 = node_ptr->getContent().pos[1];
            
                x1 = node_dest_ptr->getContent().pos[0];
                y1 = node_dest_ptr->getContent().pos[1];

                current_img.draw_line(x0, y0, x1, y1, RED);
            }
        }
    }
}

template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::init(){
	this->algo->init();
}

template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::start() {
	bool is_pressed = false;
    vector< vector< pair<int, int> > > drawing_points;
<<<<<<< HEAD
    CImg<unsigned char> current_img(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
    GNGExec<Algorithm, Trait> exe(this->algo, vector<pair<int,int>>(), GenType::UNIFORM_DISTRIBUTION);
=======
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
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d

    while (!this->window.is_closed()) {
        if(exe.isRunning()){ 
            GNGExec<Algorithm, Trait> exe(this->algo, this->to_single_vec(drawing_points), GenType::UNIFORM_DISTRIBUTION);
            vector<pair<int,int>> vector_points = this->to_single_vec(drawing_points);
            exe.next();
            this->drawExtras(this->algo->get_graph(), exe.getExecutionCount(), current_img);

            if(exe.getExecutionCount() > MAX_EXECUTIONS)
                exe.setRunning(false);


            current_img.display(this->window);
            cimg::wait(1);
        }else{
            //When click is pressed saves point and realeases it when click is released
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
                            current_img.draw_line(p.first, p.second, last_pos.first, last_pos.second, WHITE);
                            last_pos = p;        
                        }
                    }

                    current_img.display(this->window);
                }
                //If there is a queued line push it to the accepted drawing
                if(tmp_cord_holder.size() != 0)
                    drawing_points.push_back(tmp_cord_holder);
            } 
            //When receiving and ESC key clears the image and the vector of points
            if(this->window.is_key() && this->window.is_keyESC()){
                drawing_points.clear();
                current_img.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
                cout << "cleared " << endl;
            }
            this->drawFigure(this->to_single_vec(drawing_points), current_img);
            current_img.display(this->window);
            this->getInput(exe);
            
        }
        
    }
}

template <template <class> class Algorithm, class Trait>
void PictureGNGContainer<Algorithm, Trait>::init(){ 
	this->algo->init();
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
        this->pic_vector.push_back(tmp_pair);
    }
    file.close();
} 

template <template <class> class Algorithm, class Trait>
void PictureGNGContainer<Algorithm, Trait>::start(){
<<<<<<< HEAD
=======
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
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d

    CImg<unsigned char> current_img(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
    GNGExec<Algorithm, Trait> exe(this->algo, this->pic_vector, GenType::UNIFORM_DISTRIBUTION);

    while (!this->window.is_closed()) {
        if (exe.isRunning()) {
		    exe.next();
            if(exe.getExecutionCount() >= 100)
                exe.setRunning(false);
            this->drawExtras(this->algo->get_graph(), this->algo->get_iteracion(), current_img);
            current_img.display(this->window);
            cimg::wait(1);
		}else{
	        this->getInput(exe);
        }
            
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
<<<<<<< HEAD
			MUIG inpt_gen(drawing_points);
=======
			MovingUniformDistributionInputGenerator<Trait>inpt_gen = MovingUniformDistributionInputGenerator<Trait>(drawing_points);
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
            while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 100000){
                CImg<unsigned char> currentImg(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
                
                drawing_points = inpt_gen.getInput();
<<<<<<< HEAD

                this->drawPicture(drawing_points, currentImg);

=======

                this->drawPicture(drawing_points, currentImg);

>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
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
template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::init(){
	this->algo->init();
    frame_list.load_gif_external("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/video/video.gif");
    if(DEBUG) cout << "Loaded GIF to memory" << endl;
}

template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::start(){ 
    GNGExec<Algorithm, Trait> exe(this->algo, vector<pair<int,int>>(), GenType::UNIFORM_DISTRIBUTION);
    this->current_frame = this->frame_list.data();
    //this->current_frame = *iter;

    while (!this->window.is_closed()) {
        current_frame->display(this->window);
        if(exe.isRunning()){
           vector<pair<int,int>> drawing_points;
           exe.setInputData(drawing_points);
           exe.next();
           this->drawExtras(this->algo->get_graph(), exe.getExecutionCount(),*current_frame);
        }
        if(exe.getExecutionCount() % 10)
            current_frame++;

        this->getInput(exe);
        //TODO CHANGE FRAMES
    }
}

template< template <class> class Algorithm, class Trait>
vector<pair<int,int>> VideoGNGContainer<Algorithm, Trait>::getBinaryPoints(CImg<unsigned char> img){
    vector<pair<int,int>> binary_points;
    cimg_forXY(img, x, y){
        int R,G,B,greyscale;
        R = img(x, y, 0, 0);
        G = img(x, y, 0, 1);
        B = img(x, y, 0, 2);
        greyscale = (int) (0.299*R + 0.587*G + 0.114*B);
        if(greyscale > 120)
            binary_points.push_back(make_pair(x,y));
        //cout << greyscale << endl;
    }
    return binary_points;
}
template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::binarizeImg(CImg<unsigned char> &img){
    img.quantize(16).normalize(0, 1).cut(0.2f, 0.8f).threshold(0.5f).normalize(0, 255);

}
