#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"

#define DEBUG 1

#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 320


template<class GNGTrait>
auto GNGAlgorithm<GNGTrait>::findMaxError(Graph<GNGTrait> &graph){
	NodePtr max_node = nullptr;
	for(NodePtr node : graph.getNodesVector()){
		if(max_node == nullptr)
			max_node = node;
		else
			if(node->getContent().error > max_node->getContent().error)
				max_node = node;
	}
	return max_node;
}
//Finds the neighbor of node with the highest error value
template<class GNGTrait>
auto GNGAlgorithm<GNGTrait>::findMaxErrorLink(NodePtr node){
	NodePtr max_node = nullptr;
	for(EdgePtr e : node->getEdges()) {
		if (max_node == nullptr)
			max_node = e->getDest(node);
		else
			if(e->getDest(node)->getContent().error > max_node->getContent().error)
				max_node = e->getDest(node);
	}
	return max_node;
}
template<class GNGTrait>
void GNGAlgorithm<GNGTrait>::init() {
	//iteracion = 0;
	base_graph.clear();
	double pos1[2];
	double pos2[2];

	//Creamos dos puntos al azar en el espacio de la pantalla
	pos1[0] = rand() % SCREEN_HEIGHT;
	pos1[1] = rand() % SCREEN_WIDTH;
	pos2[0] = rand() % SCREEN_HEIGHT;
	pos2[1] = rand() % SCREEN_WIDTH;

	//Creamos los contenidos basado en las posiciones e inicializamos los errores a 0
	NodeContent c1 = {{pos1[0], pos1[1]}, 0};
	NodeContent c2 = {{pos2[0], pos2[1]}, 0};

	//Insertamos los nodos al grafo
	base_graph.insertNode(c1);
	base_graph.insertNode(c2);

	//Insertamos una coneccion basica con edad 0
	base_graph.insertEdge(0, c1, c2, false);
}
template <class GNGTrait>
bool GNGAlgorithm<GNGTrait>::isConnected(NodePtr node1, NodePtr node2){
	for(EdgePtr e : node1->getEdges()){
		if(e->getDest(node1) == node2)
			return true;
	}
	return false;
}
template<class GNGTrait>
double GNGAlgorithm<GNGTrait>::getDistance(NodePtr node, sf::Vertex input){
	return 
			pow(node->getContent().pos[0] - input.position.x, 2) +
			pow(node->getContent().pos[1] - input.position.y, 2);
}
template<class GNGTrait>
void GNGAlgorithm<GNGTrait>::exec(sf::Vertex input){
	NodePtr smallestNodes[2];
	smallestNodes[0] = nullptr;
	smallestNodes[1] = nullptr;

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos
	
	double dist_small2_inpt;
	for(NodePtr node : base_graph.getNodesVector()){
		if(smallestNodes[0] == nullptr)
			smallestNodes[0] = node;
		else if(getDistance(node, input) < getDistance(smallestNodes[0], input)){
			smallestNodes[1] = smallestNodes[0];
			smallestNodes[0] = node;
		}
		else if(smallestNodes[1] == nullptr)
			smallestNodes[1] = node;

		else if(node < smallestNodes[1]  && node != smallestNodes[0])
			smallestNodes[1] = node;
	}	
	assert(smallestNodes[0] != nullptr && smallestNodes[1] != nullptr && smallestNodes[0] != smallestNodes[1]);


	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante E_W multiplicado por el vector de diferencia entre
	//Input y el menor nodo

	double nueva_posicion[2] = {
			smallestNodes[0]->getContent().pos[0] + E_W *(input.position.x - smallestNodes[0]->getContent().pos[0]),
			smallestNodes[0]->getContent().pos[1] + E_W * (input.position.y - smallestNodes[0]->getContent().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	NodeContent c1 = {{nueva_posicion[0], nueva_posicion[1]},
				   smallestNodes[0]->getContent().error + getDistance(smallestNodes[0], input),
       				smallestNodes[1].getContent().error - smallestNodes[0].getContent().error
				       	+ smallestNodes[0].getConten().U}; //TODO Modificacion add Utility

	smallestNodes[0]->setContent(c1);


	for(EdgePtr e : smallestNodes[0]->getEdges()){
		NodePtr node_destino = e->getDest(smallestNodes[0]);
		
		//Si se encuentra una coneccion entre los 2 menores nodos, cambia la edad a 0
		if(node_destino == smallestNodes[1])
			e->setContent(0);

		//Eliminamos la arista si exede el maximo age
		if(e->getContent() > MAX_AGE) {
			base_graph.deleteEdge(smallestNodes[0]->getContent(),
							  e->getDest(smallestNodes[0])->getContent(),
							  e->getContent());
		}

		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante E_N
		double nueva_posicion_vecino[2] = {
				node_destino->getContent().pos[0] +  E_N * (input.position.x - node_destino->getContent().pos[0]),
				node_destino->getContent().pos[1] +  E_N * (input.position.y - node_destino->getContent().pos[1])
		};

		NodeContent nc_vecino = {{nueva_posicion_vecino[0], nueva_posicion_vecino[1]},
						   smallestNodes[0]->getContent().error};

		node_destino->setContent(nc_vecino);


		//TODO Probar diferentes valores para AGE
		e->setContent(e->getContent()+1); 

	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!isConnected(smallestNodes[0], smallestNodes[1]))
		base_graph.insertEdge(0, smallestNodes[0]->getContent(), smallestNodes[1]->getContent(), false);

	//Busca a lo largo de todos los nodos y si ya no tienen aristas los elimina del grafo
	for(NodePtr node : base_graph.getNodesVector()){
		if(node->getEdges().empty())
			base_graph.deleteNode(node->getContent());
	}

	//cout << base_graph.size() << " " << MAX_NODES << endl;
	
	
        //cout << SIGMA << " " << iteracion << endl << endl;	
	if(iteracion % SIGMA == 0 && base_graph.size() < MAX_NODES) {
		NodePtr max_error_node = findMaxError(base_graph);
		NodePtr max_error_neighbor = findMaxErrorLink(max_error_node);

		assert(max_error_neighbor != nullptr);
			


		//Creamps el contenido de un nuevo nodo y asignamos sus posiciones a la media
		//De las posiciones de los nodos con max_error y su max_error_neighbor

		NodeContent content_max = max_error_node->getContent();
		NodeContent content_neighbor = max_error_neighbor->getContent();

		double pos_modificada[2] = {
				(content_max.pos[0] + content_neighbor.pos[0]) / 2,
				(content_neighbor.pos[1] + content_max.pos[1]) / 2
		};
		NodeContent content = {{pos_modificada[0], pos_modificada[1]},
				content_max.error
		};

		base_graph.insertNode(content);

		//Creamos una arista entre max y su maximo vecino y el nuevo nodo
		//Asimismo eliminamos la arista entre el maximo y su vecino
		base_graph.insertEdge(0, content_max, content, false);
		base_graph.insertEdge(0, content_neighbor, content, false);

		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		base_graph.deleteEdge(content_max, content_neighbor,
						  edgePtr->getContent());

		//Decrementamos el error de ambos nodos por ALFA y los asignamos
		//Asimismo decrementamos el erro de todos los nodos de la forma
		// error_x <- error_x - BETA * error_x

		content_max.error = content_max.error * ALFA;
		content_neighbor.error = content_neighbor.error * ALFA;

		max_error_node->setContent(content_max);
		max_error_neighbor->setContent(content_neighbor);

		for(NodePtr node_tmp : base_graph.getNodesVector()){
			NodeContent content_tmp = node_tmp->getContent();
			content_tmp.error = content_tmp.error -  BETA * content_tmp.error;
			node_tmp->setContent(content_tmp);
		}
	}
	iteracion++;

}

template <class GNGTrait>
InputGenerator<GNGTrait>::InputGenerator(vector<sf::VertexArray> vec){
	for(auto vertArray : vec){
		for(int x = 0;x < vertArray.getVertexCount();x++){
			sf::Vertex v = vertArray[x];
			pos_vector.push_back(v);
		}
	}
}
template<class GNGTrait>
unsigned long InputGenerator<GNGTrait>::size(){
	return pos_vector.size();
}

template <class GNGTrait>
sf::Vertex DefaultInputGenerator<GNGTrait>::pop(){
	auto item = this->pos_vector.back();
	this->pos_vector.pop_back();
	return item;	
}
template <class GNGTrait>
sf::Vertex UniformDistributionInputGenerator<GNGTrait>::pop(){
	int gen =  dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
	sf::Vertex toReturn = this->pos_vector[gen];
	this->pos_vector.erase(this->pos_vector.begin()+gen);
	return toReturn;
}

template<class GNGTrait>
void GNGContainer<GNGTrait>::init(){
	window.create(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Growing Neural Gas");
	algo.init();
}

template<class GNGTrait>
void GNGContainer<GNGTrait>::start() {
	bool is_pressed = false;
	vector<sf::VertexArray> figures;
	sf::VertexArray current_figure(sf::LinesStrip, 100);
	//Ciclo de ejecucion del programa
	while (window.isOpen()) {
		sf::Event event;

		//Ciclo de
		if (is_running) {
			UniformDistributionInputGenerator<GNGTrait>inpt_gen = UniformDistributionInputGenerator<GNGTrait>(figures);
			while(inpt_gen.size() > 0 && algo.get_iteracion() < 40000){

				window.clear(sf::Color::Black);
				sf::Vertex tmp = inpt_gen.pop();

				algo.exec(tmp);

				Graph<GNGTrait> g = algo.get_graph();
				
				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
					sf::CircleShape node_circle(5 , 30);
					node_circle.setFillColor(sf::Color::Green);
					sf::Vector2i vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
					sf::Vector2f v_test(window.mapPixelToCoords(vi));
					//node_circle.setPosition(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
					node_circle.setPosition(v_test);
					window.draw(node_circle);

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
						
						NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
							
						sf::Vector2i start_vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
						sf::Vector2i end_vi(node_dest_ptr->getContent().pos[0], node_dest_ptr->getContent().pos[1]);
						
						sf::Vector2f start_v(window.mapPixelToCoords(start_vi));
						sf::Vector2f end_v(window.mapPixelToCoords(end_vi));
						sf::Vertex* edge = new sf::Vertex[2];
						edge[0] = sf::Vertex(start_v, sf::Color::Red);
						edge[1] = sf::Vertex(end_v, sf::Color::Red);
						if(DEBUG)	
							window.draw(edge, 2, sf::LinesStrip);
					}
				}
				sf::Text it_text;
				sf::Font font;
				font.loadFromFile("arial.ttf");

				it_text.setFont(font);
				it_text.setString(to_string(algo.get_iteracion()));
				it_text.setCharacterSize(12);
				it_text.setPosition(SCREEN_WIDTH-50, 0);
				
				window.draw(it_text);
				for(auto v : figures){
					window.draw(v);
				}

				window.display();
				sf::sleep(sf::milliseconds(1));
			}

		}

		//Ciclo de eventos
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
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
				if(is_pressed){
					int x = event.mouseMove.x;
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
						window.clear();
					}
				}

			}
		}


		window.draw(current_figure);
	

		//Logica de dibujo y demas
		/*if (pos1.x != -1 && pos1.y != -1 && pos2.x != -1 && pos2.y != -1) {
			sf::Vertex line[] = {
					sf::Vertex(sf::Vector2f(pos1.x, pos1.y), sf::Color::White),
					sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::White)
			};
			window.draw(line, 2, sf::Lines);
		}*/



		//nodes.push_back(draw_node(x, y)); Añadir un node a la cola de dibujo
		//edges.push_back(draw_edge(x, y)); Añadir un edge a la cola de dibujo


		window.display();
	}
}

/*template<class GNGTrait>
void UniformDistributionInputGenerator<GNGTrait>::move_input(int amount_x, int amount_y, sf::Window& window){
	for(auto pos : this->pos_vector){
		int sign = 1;
		if(pos.position.x + amount_x >= window.getSize().x){
			sign*-1;
		}
		pos.position.x += amount_x*sign;
	}
}*/
template<class GNGTrait>
void GNGContainer<GNGTrait>::start_moving() {
	bool is_pressed = false;
	vector<sf::VertexArray> figures;
	sf::VertexArray current_figure(sf::LinesStrip, 100);
	//Ciclo de ejecucion del programa
	while (window.isOpen()) {
		sf::Event event;

		//Ciclo de
		if (is_running) {
			UniformDistributionInputGenerator<GNGTrait>inpt_gen = UniformDistributionInputGenerator<GNGTrait>(figures);
			while(inpt_gen.size() > 0 && algo.get_iteracion() < 40000){
				//if(algo.get_iteracion() % 400 == 0)
				//	inpt_gen.move_input(5, 0, window);	
				window.clear(sf::Color::Black);
				sf::Vertex tmp = inpt_gen.pop();

				algo.exec(tmp);

				Graph<GNGTrait> g = algo.get_graph();
				
				//Draws each edge
				for(NodePtr node_ptr : g.getNodesVector()){
				
					sf::CircleShape node_circle(5 , 30);
					node_circle.setFillColor(sf::Color::Green);
					sf::Vector2i vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
					sf::Vector2f v_test(window.mapPixelToCoords(vi));
					//node_circle.setPosition(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
					node_circle.setPosition(v_test);
					window.draw(node_circle);

					//Draws each edge
					for(EdgePtr edge_ptr : node_ptr->getEdges()){
						
						NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);
							
						sf::Vector2i start_vi(node_ptr->getContent().pos[0], node_ptr->getContent().pos[1]);
						sf::Vector2i end_vi(node_dest_ptr->getContent().pos[0], node_dest_ptr->getContent().pos[1]);
						
						sf::Vector2f start_v(window.mapPixelToCoords(start_vi));
						sf::Vector2f end_v(window.mapPixelToCoords(end_vi));
						sf::Vertex* edge = new sf::Vertex[2];
						edge[0] = sf::Vertex(start_v, sf::Color::Red);
						edge[1] = sf::Vertex(end_v, sf::Color::Red);
						if(DEBUG)	
							window.draw(edge, 2, sf::LinesStrip);
					}
				}
				sf::Text it_text;
				sf::Font font;
				font.loadFromFile("arial.ttf");

				it_text.setFont(font);
				it_text.setString(to_string(algo.get_iteracion()));
				it_text.setCharacterSize(12);
				it_text.setPosition(SCREEN_WIDTH-50, 0);
				
				window.draw(it_text);
				for(auto v : figures){
					window.draw(v);
				}

				window.display();
				sf::sleep(sf::milliseconds(1));
			}

		}

		//Ciclo de eventos
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
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
				if(is_pressed){
					int x = event.mouseMove.x;
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
						window.clear();
					}
				}

			}
		}


		window.draw(current_figure);
	

		//Logica de dibujo y demas
		/*if (pos1.x != -1 && pos1.y != -1 && pos2.x != -1 && pos2.y != -1) {
			sf::Vertex line[] = {
					sf::Vertex(sf::Vector2f(pos1.x, pos1.y), sf::Color::White),
					sf::Vertex(sf::Vector2f(pos2.x, pos2.y), sf::Color::White)
			};
			window.draw(line, 2, sf::Lines);
		}*/



		//nodes.push_back(draw_node(x, y)); Añadir un node a la cola de dibujo
		//edges.push_back(draw_edge(x, y)); Añadir un edge a la cola de dibujo


		window.display();
	}

}
