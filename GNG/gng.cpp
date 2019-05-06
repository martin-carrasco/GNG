#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"
#include <sstream>

#define DEBUG 1

template<class Trait, class Algorithm>
void GNGContainer<Trait, Algorithm>::drawNode(int x, int y){
    sf::CircleShape node_circle(5 , 30);
    node_circle.setFillColor(sf::Color::Green);
    sf::Vector2i vi(x, y);
    sf::Vector2f v_test(this->window.mapPixelToCoords(vi));
    //node_circle.setPosition(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
    node_circle.setPosition(v_test);
    this->window.draw(node_circle);
}
template <class Trait, class Algorithm>
void GNGContainer<Trait, Algorithm>::drawCounter(){
    sf::Text it_text;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    it_text.setFont(font);
    it_text.setString(to_string(this->algo.get_iteracion()));
    it_text.setCharacterSize(12);
    it_text.setPosition(SCREEN_WIDTH-50, 0);
    (this->window).draw(it_text);
}
template <class Trait, class Algorithm>
void GNGContainer<Trait, Algorithm>::drawEdge(EdgePtr edge_ptr, NodePtr node_ptr){
	NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
        
    sf::Vector2i start_vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
    sf::Vector2i end_vi(node_dest_ptr->getContent().pos[0], node_dest_ptr->getContent().pos[1]);
    
    sf::Vector2f start_v(this->window.mapPixelToCoords(start_vi));
    sf::Vector2f end_v(this->window.mapPixelToCoords(end_vi));
    sf::Vertex* edge = new sf::Vertex[2];
    edge[0] = sf::Vertex(start_v, sf::Color::Red);
    edge[1] = sf::Vertex(end_v, sf::Color::Red);
    if(DEBUG)	
        this->window.draw(edge, 2, sf::LinesStrip);
}

template<class Trait, class Algorithm>
void GNGContainer<Trait, Algorithm>::init(){
	this->window.create(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Growing Neural Gas");
	this->algo.init();
}

template<class Trait, class Algorithm>
void GNGContainer<Trait, Algorithm>::start() {
	bool is_pressed = false;
	vector<sf::VertexArray> figures;
	sf::VertexArray current_figure(sf::LinesStrip, 100);
	//Ciclo de ejecucion del programa
	while (this->window.isOpen()) {
		sf::Event event;

		//Ciclo de
		if (is_running) {
			UniformDistributionInputGenerator<Trait>inpt_gen = UniformDistributionInputGenerator<Trait>(figures);
			while(inpt_gen.size() > 0 && this->algo.get_iteracion() < 40000){

				this->window.clear(sf::Color::Black);
				sf::Vertex tmp = inpt_gen.pop();
				this->algo.exec(tmp);

				Graph<Trait> g = this->algo.get_graph();
				
				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
				    this->drawNode(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);	

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
						
				        this->drawEdge(edge_ptr, node_ptr);	
					}
				}
                drawCounter(); 
                this->window.draw(it_text);
				for(auto v : figures){
					this->window.draw(v);
				}

				this->window.display();
				sf::sleep(sf::milliseconds(1));
			}

		}

		//Ciclo de eventos
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				this->window.close();
			else if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Left)
					is_pressed = true;
			}
			else if(event.type == sf::Event::MouseButtonReleased){
				if(event.mouseButton.button == sf::Mouse::Left){
					is_pressed = false;
					figures.push_back(current_figure);
					current_figure.clear();
				}
			}

			else if(event.type == sf::Event::MouseMoved){
				if(is_pressed){ int x = event.mouseMove.x;
					int y = event.mouseMove.y;
					sf::Vector2i vec(x, y);
					sf::Vector2f pos = window.mapPixelToCoords(vec);
					current_figure.append(sf::Vertex(pos));
				}

			}
			else if (event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Space)
					is_running = !is_running;
				else if (event.key.code = sf::Keyboard::Escape) {
					if(!is_running){
						current_figure.clear();
						figures.clear();
						this->window.clear();
					}
				}

			}
		}
		this->window.draw(current_figure);
		this->window.display();
	}
}
template <class Trait, Algorithm>
void PictureGNGContainer<Trait, Algorithm>::init(){ 
	this->window.create(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Growing Neural Gas");
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
template <class Trait, Algorithm>
void PictureGNGContainer<Trait, Algorithm>::start(){
    vector<sf::VertexArray> v_transform;
	sf::VertexArray figures(sf::Points, 640);
    for(const pair<int,int>& p : this->pic_vector){
        sf::Vector2i vec(p.first, p.second);
        sf::Vector2f pos(vec);
        vec = this->window.mapCoordsToPixel(pos);
        sf::Vector2f pos_2(vec);
        sf::Vertex tmp_vec = sf::Vertex(pos_2);
        tmp_vec.color = sf::Color::White;
        figures.append(tmp_vec);
    } 
    v_transform.push_back(figures);
    //Ciclo de ejecucion del programa
	while (this->window.isOpen()) {
		sf::Event event;
		//Ciclo de
		if (1) {
			DefaultInputGenerator<Trait>inpt_gen = DefaultInputGenerator<Trait>(v_transform);
			while(inpt_gen.size() > 0 && algo.get_iteracion() < 40000){
                
				this->window.clear(sf::Color::Black); 
                sf::Vertex tmp = inpt_gen.pop();

				this->algo.exec(tmp);

				Graph<Trait> g = this->algo.get_graph();
				
				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
                    this->drawNode(node_ptr);

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
				        this->drawNode(edge_ptr, node_ptr);	
					}
				}
                drawCounter(this->window);
				this->window.draw(figures);
				this->window.display();
				sf::sleep(sf::milliseconds(1));
			}

		}

		//Ciclo de eventos
		while (this->window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				this->window.close();	
		}
        
	    this->window.display();
	}

}
