#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"
#include <sstream>
#include <SFML/Graphics.hpp>

#define DEBUG 1
#define MAX_EXECUTIONS 5000

using namespace cimg_library;

static constexpr int RED[] = {255, 0, 0};
static constexpr int GREEN[] = {0, 255, 0};
static constexpr int WHITE[] = {255, 255, 255};
static constexpr int BLUE[] = {0, 0, 255};

enum InputType {
    TOGGLE_RUNNING,
    CLICK_ON,
    CLICK_OFF,
    CLEAR,
    MOUSE_MOVE,
    QUIT;
};

template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::imageWait(int time){
   sf::sleep(sf::milliseconds(1));
}

template< template <class> class Algorithm, class Trait>
sf::VertexArray GNGContainer<Algorithm, Trait>::mergeVertex(sf::PrimitiveType primitive, vector< sf::VertexArray > vec){
    sf::VertexArray final_vertices(primitive, 1);

    for(auto current_vertices : vec){
        for(auto current_vertex : current_vertices){
            final_vertices.append(current_vertex);
        }
    }
    return final_vertices;
}

template< template <class> class Algorithm, class Trait>
InputType GNGContainer<Algorithm, Trait>::getInput(sf::Event event){
    while(window.pollEvent(event)){
        if(event.type == sf::Event::Closed)
            return InputType::QUIT;

        if(event.type == sf::Event::KeyPressed){
            if(event.key.code == sf::Keyboard::Space)           
                return InputType::TOGGLE_RUNNING;
            else if(event.key.code == Sf::Keyboard::Esc)
                return InputType::CLEAR;
        }

        if(event.type == sf::Event::MouseButtonPressed)
            return InputType::CLICK_ON; 
        
        if(event.type == sf::Event::MouseButtonReleased)
            return InputType::CLICK_OFF;

        if(event.type == sf::Event::MouseMoved)
            return InputType::MOUSE_MOVE;
    } 
}
template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawPicture(sf:VertexArray points){
    this->window.draw(points);
}

template< template <class> class Algorithm, class Trait>
void GNGContainer<Algorithm, Trait>::drawExtras(Graph<Trait> graph, int exec_count){
    sf::Text it_text;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    it_text.setFont(font);
    it_text.setString(to_string(exec_count);
    it_text.setCharacterSize(12);

    it_text.setPosition(SCREEN_WIDTH-50, 0);
    this->window.draw(it_tex);

    for(NodePtr node_ptr : graph.getNodesVector()){

        sf::CircleShape node_circle(5 , 30);
        node_circle.setFillColor(sf::Color::Green);
        sf::Vector2i vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
        sf::Vector2f v_test(this->window.mapPixelToCoords(vi));
        node_circle.setPosition(v_test);
        this->window.draw(node_circle);

        for(EdgePtr edge_ptr : node_ptr->getEdges()){

			if(DEBUG){			
                NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
                    
                sf::Vector2i start_vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
                sf::Vector2i end_vi(node_dest_ptr->getContent().pos[0], node_dest_ptr->getContent().pos[1]);
                
                sf::Vector2f start_v(this->window.mapPixelToCoords(start_vi));
                sf::Vector2f end_v(this->window.mapPixelToCoords(end_vi));
                sf::Vertex* edge = new sf::Vertex[2];
                edge[0] = sf::Vertex(start_v, sf::Color::Red);
                edge[1] = sf::Vertex(end_v, sf::Color::Red);
                
                this->window.draw(edge, 2, sf::LinesStrip);
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

    //Vector of lines drawn
    vector< sf::VertexAray > drawing_points;
    //Current line beign drawn
    sf::VertexArray tmp_holder(sf::LineStrip, 1);

    GNGExec<Algorithm, Trait> exe(this->algo, this->mergeVertex(drawing_points), GenType::UNIFORM_DISTRIBUTION);

    while (this->window.isOpen()) {
        sf::Event event;
        sf::VertexArray vertices = this->mergeVertex(drawing_points);
        this->drawPicture(vertices);

        this->window.clear(sf::Color::Black);
        if(exe.isRunning()){ 
            exe.next();
            this->drawExtras(this->algo->get_graph(), exe.getExecutionCount(), current_img);

            if(exe.getExecutionCount() > MAX_EXECUTIONS)
                exe.setRunning(false);

            this->imageWait(1);

        }else{
            InputType inpt = this->getInput(event);
            switch(inpt)
            {
                case TOGGLE_RUNNING:
                    exe.setRunning(true);
                    break;
                case CLICK_ON:
                    is_pressed = true;
                case CLICK_OFF:
                    if(is_pressed){
                        drawing_points.push_back(tmp_holder);
                        tmp_holder.clear();
                    }
                    is_pressed = false;
                    break;
                case QUIT:
                    this->window.close();
                    break;
                case CLEAR:
                    drawing_points.clear();
                    tmp_holder.clear();
                    this->window.clear();
                    break;
                case MOUSE_MOVE:
                    if(is_pressed){
                        int x, y;
                        
                        x = event.mouseMove.x;
                        y = event.mouseMove.y;
                        sf::Vertex2f cord_vertex(x, y);

                        tmp_holder.append(cord_vertex);
                    }
                    break; 
        }
        this->window.display();
        
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
        int x, y;
        x = atoi(tokens[0].c_str());
        y = atoi(tokens[1].c_str());
        sf::Vertex2f tmp_vertex(x, y);
        this->pic_vertices.append(tmp_vertex);
    }
    file.close();
} 

template <template <class> class Algorithm, class Trait>
void PictureGNGContainer<Algorithm, Trait>::start(){
    GNGExec<Algorithm, Trait> exe(this->algo, this->pic_vertices, GenType::UNIFORM_DISTRIBUTION);

    this->window.clear();
    this->drawPicture(this->pic_vertices);

    while (this->window.isOpen() {
        if (exe.isRunning()) {
		    exe.next();
            if(exe.getExecutionCount() >= MAX_EXECUTIONS)
                exe.setRunning(false);

            this->drawExtras(this->algo->get_graph(), exe.getExecutionCount();
            this->imageWait(1);
		}else{
	        InputType inpt = this->getInput(exe);
            switch(inpt)
            {
                case QUIT:
                    this->window.close();
                    break;
                case TOGGLE_RUNNING:
                    exe.setRunning(!(exe.getRunning()));
                    break;
                
            }
        }    
        this->window.display();
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
        int x, y;
        x = atoi(tokens[0].c_str());
        y = atoi(tokens[1].c_str());
        sf::Vertex tmp_vertex(x, y);

        this->pic_vector.push_back(tmp_vertex);
    }
    file.close();
} 
template <template <class> class Algorithm, class Trait>
void MovingPictureGNGContainer<Algorithm, Trait>::start(){

    sf::VertexArray current_points(sf::Points, 1);
    vector<pair<int, int>> drawing_points; 
    drawing_points = this->pic_vector;
    while (this->window.isOpen()) {
        if (this->is_running) {
			MUIG inpt_gen(drawing_points);
            while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 100000){
                
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
	this->algo->init(); //TODO FIGURE OUT HOW TO LEAD VIDEO
    frame_list.load_video("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/video/output.mp4");
    if(DEBUG) cout << "Loaded GIF to memory" << endl;
}

template< template <class> class Algorithm, class Trait>
void VideoGNGContainer<Algorithm, Trait>::start(){ 
    sf::VertexArray empty_vertices;
    GNGExec<Algorithm, Trait> exe(this->algo, empty_vertices, GenType::UNIFORM_DISTRIBUTION);

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
