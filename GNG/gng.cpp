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

template< class Trait>
vector<pair<int,int>> GNGContainer<Trait>::to_single_vec(vector< vector< pair<int, int> > > vec){
    vector<pair<int, int>> toReturn;
    for(auto current_vector : vec){
        for(auto current_pair : current_vector){
            toReturn.push_back(current_pair);
        }
    }
    return toReturn;
}

template<class Trait>
void GNGContainer<Trait>::init(){}

template<class Trait>
void GNGContainer<Trait>::start() {
    ImgManager<Trait> img_manager(this->window);
    vector< vector< pair<int, int> > > drawing_points;
    vector< pair<int,int> > cached_points;
    GNGExec<GNGAlgorithm, Trait> exe(make_pair(SCREEN_WIDTH, SCREEN_HEIGHT), this->to_single_vec(drawing_points), UNIFORM_DISTRIBUTION);
	bool is_pressed = false;
    bool is_drawn = false;
    bool cache_updated = true;
    while (!this->window->is_closed()) {
       
        while(!is_drawn){

            //When click is pressed saves point and realeases it when click is released
            if(this->window->button()){
                vector<pair<int, int>> tmp_cord_holder;
                while( this->window->button() & 1){ 
                    pair<int, int> click_pair = make_pair(this->window->mouse_x(), this->window->mouse_y());         
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
                            img_manager.drawLine(p, last_pos, WHITE);
                            last_pos = p;        
                        }
                    }
                }
                //If there is a queued line push it to the accepted drawing
                if(tmp_cord_holder.size() != 0)
                    drawing_points.push_back(tmp_cord_holder);
            }

            //When it receives the space key it changes the running state and uploads the input data collected so far
            if(this->window->is_key() && this->window->is_keySPACE()){
                exe.setRunning(true);
                exe.setInputData(this->to_single_vec(drawing_points));
                is_drawn = true;
            }
            //When receiving and ESC key clears the image and the vector of points
            if(this->window->is_key() && this->window->is_keyESC()){
                drawing_points.clear();
                img_manager.clearMain();
                if(DEBUG) cout << "cleared " << endl;
            }
            img_manager.drawPicture(this->to_single_vec(drawing_points));           
        }

        if(exe.getExecutionCount() < MAX_EXECUTIONS){

            //Calls the algorithm to get an input and execute next loop if not reached the maximum iterations
            exe.next();
            
            if(!cache_updated){
                cached_points = this->to_single_vec(drawing_points);
                cache_updated = true;
                img_manager.drawPicture(cached_points);
            }else{
                img_manager.drawPicture(cached_points);
            }

            //Calls the executor to draw everything to the screen
            img_manager.drawExtras(exe.getNodes(), exe.getExecutionCount());

            //Toggles running 
            if(this->window->is_key() && this->window->is_keySPACE()){
                exe.setRunning(!exe.isRunning());
            }

            cimg::wait(1);
        }  
    }
}

template <class Trait>
void PictureGNGContainer<Trait>::init(){ 
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

template <class Trait>
void PictureGNGContainer<Trait>::start(){
    ImgManager<Trait>* img_manager = new ImgManager<Trait>(this->window);
    GNGExec<GNGAlgorithm, Trait>* exe = new GNGExec<GNGAlgorithm, Trait>(make_pair(SCREEN_WIDTH, SCREEN_HEIGHT), this->pic_vector, GenType::UNIFORM_DISTRIBUTION);
    img_manager->drawPicture(this->pic_vector);
    while (!this->window->is_closed()) {
        if(exe->getExecutionCount() < MAX_EXECUTIONS){
            exe->next();
            img_manager->drawExtras(exe->getNodes(), exe->getExecutionCount());
           // cimg::wait(1);
        } 

        //Toggles running 
        if(this->window->is_key() && this->window->is_keySPACE()){ 
            exe->setRunning(true);
        }

    }
    delete exe;
    delete img_manager;
}

template <class Trait>
void MovingPictureGNGContainer<Trait>::init(){ 
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
template <class Trait>
void MovingPictureGNGContainer<Trait>::start(){
   /* vector<pair<int, int>> drawing_points; 
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
    */
}
template< class Trait>
void VideoGNGContainer<Trait>::init(){
    frame_list.load_video("/home/martin/Documents/Utec/Clases/ADA/Grafo-GNG/video/output.mp4");
    if(DEBUG) cout << "Loaded video to memory" << endl;
}

template< class Trait>
void VideoGNGContainer<Trait>::start(){
    CImgList<unsigned char>::iterator it = this->frame_list.begin();
    ImgManager<Trait>* img_manager = new ImgManager<Trait>(this->window, &(*it));
    vector< pair<int,int> > drawing_points = this->parseFrame(*it);
    GNGExec<UGNGAlgorithm, Trait>* exe = new GNGExec<UGNGAlgorithm, Trait>(make_pair(SCREEN_WIDTH, SCREEN_HEIGHT), drawing_points, GenType::UNIFORM_DISTRIBUTION); 

    while (!this->window->is_closed()) {
        if(exe->getExecutionCount() < MAX_EXECUTIONS){
            if(exe->getExecutionCount() % 500 == 0){
               it++;
               if(it == this->frame_list.end()){
                   cimg::wait(10000);
                   cout << "Got to last frame!" << endl;
               }
               img_manager->changeImage( &(*it));
               this->parseFrame(*it);
               exe->setInputData(drawing_points);
            }
            exe->next();
            img_manager->drawExtras(exe->getNodes(), exe->getExecutionCount());


            //Toggles running 
            if(this->window->is_key() && this->window->is_keySPACE()){ 
                exe->setRunning(true);
            }
        }
    }
}

template< class Trait>
void VideoGNGContainer<Trait>::binarizeImg(CImg<unsigned char> &img){
    img.quantize(16).normalize(0, 1).cut(0.2f, 0.8f).threshold(0.5f).normalize(0, 255);

}

template< class Trait>
vector<pair<int,int>> VideoGNGContainer<Trait>::getBinaryPoints(CImg<unsigned char> img){
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

template< class Trait>
vector< pair<int,int> > VideoGNGContainer<Trait>::parseFrame(CImg<unsigned char> frame){
    vector< pair<int,int> > black_points;
    cimg_forXY(frame, x, y){
        int R, G, B;
        R = frame(x, y, 0, 0);
        G = frame(x, y, 0, 1);
        B = frame(x, y, 0, 2);
//        cout << "X: " << x << " Y: " << y << endl;
        if(R == 0 && G == 0 && B == 0)
            black_points.push_back(make_pair(x, y));
        //cout << "RGB -> " << R << "," << G << "," << B << endl;  
    }
    return black_points;
}
