#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"
#include "img_manager.h"
#include <sstream>

#define DEBUG 1
#define MAX_EXECUTIONS 40000

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
void GNGContainer<Algorithm, Trait>::getInput(GNGExcec<Algorithm, Trait> exe){

    vector< vector< pair<int,int> > > drawing_points;
    //When receiving and ESC key clears the image and the vector of points
    if(this->window.is_key() && this->window.is_keyESC()){
        drawing_points.clear();
        current_img.assign(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 3, 0);
        cout << "cleared " << endl;
    }
    
    if(this->window.is_key() && this->window.is_keySPACE()){
        exe.setRunning(true);
        exe.setInputData(this->to_single_vec(drawing_points));
    }

}
template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::init(){
	this->algo->init();
}

template<template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::start() {
    ImageManger<Trait> img_manager(SCREEN_WIDTH, SCREEN__HEIGHT);
    vector< vector< pair<int, int> > > drawing_points;
    GNGExec<Algorithm, Trait> exe(this->algo, this->to_single_vec(drawing_points), UNIFORM_DISTRIBUTION);
	bool is_pressed = false;
    bool is_drawn = false;
    while (!this->window.is_closed()) {
        
        if(exe.getExecutionCount() < MAX_EXECUTIONS){
            //Calls the executor to draw everything to the screen
            img_manager.drawFigure();
            img_manager.drawExtras();

            //Calls the algorithm to get an input and execute next loop if not reached the maximum iterations
            exe.next();

            //Calls the input to get and handle it
            this->getInput(exe);

            cimg::wait(1);
        }

        if(!is_drawn){
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
            this->drawFigure(this->to_single_vec(drawing_points));
            current_img.display(this->window);           
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
	        //TODO add get input to set running
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
			MUIG inpt_gen(drawing_points);
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
template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::init(){
	this->algo->init();
    frame_list.load_video("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/video/output.mp4");
    if(DEBUG) cout << "Loaded video to memory" << endl;
}

template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::start(){
    this->current_frame = this->frame_list.data();
    vector< pair<int,int> > drawing_points = this->parseFrame(*current_frame);
    GNGExec<Algorithm, Trait> exe(this->algo, drawing_points, GenType::UNIFORM_DISTRIBUTION);
    
    exe.setRunning(true);

    while (!this->window.is_closed()) {

        if(exe.isRunning()){
            CImg<unsigned char> draw_frame = *current_frame;
           exe.next();
           this->drawExtras(this->algo->get_graph(), exe.getExecutionCount(),draw_frame);
           draw_frame.display(this->window);
           cimg::wait(1);
        }
        if(exe.getExecutionCount() % 10000 == 0){
            current_frame++;

            if(current_frame == nullptr){
                cout << "Last frame passed!" << endl;
                cimg::wait(1000);
                break;
            }
            drawing_points = this->parseFrame(*current_frame);
            exe.setInputData(drawing_points);
        }

        //this->getInput(exe);
    }
}

template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::binarizeImg(CImg<unsigned char> &img){
    img.quantize(16).normalize(0, 1).cut(0.2f, 0.8f).threshold(0.5f).normalize(0, 255);

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
vector< pair<int,int> > VideoGNGContainer<Algorithm, Trait>::parseFrame(CImg<unsigned char> frame){
    vector< pair<int,int> > black_points;
    cimg_forXY(frame, x, y){
        int R, G, B;
        R = frame(x, y, 0, 0);
        G = frame(x, y, 0, 1);
        B = frame(x, y, 0, 2);
        cout << "X: " << x << " Y: " << y << endl;
        if(R == 0 && G == 0 && B == 0)
            black_points.push_back(make_pair(x, y));
        //cout << "RGB -> " << R << "," << G << "," << B << endl;  
    }
    return black_points;
}
