#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "gng.h"


#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 320


template<class GNGTrait>
auto GNGAlgorithm<GNGTrait>::find_max_error(Graph<GNGTrait> &graph){
	NodePtr max_node = nullptr;
	for(NodePtr node : graph.get_nodesVector()){
		if(max_node == nullptr)
			max_node = node;
		else
			if(node->getNode_content().error > max_node->getNode_content().error)
				max_node = node;
	}
	return max_node;
}
//Finds the neighbor of node with the highest error value
template<class GNGTrait>
auto GNGAlgorithm<GNGTrait>::find_max_error_connection(NodePtr node){
	NodePtr max_node = nullptr;
	for(EdgePtr e : node->getEdges_list()) {
		if (max_node == nullptr)
			max_node = e->getDest(node);
		else
			if(e->getDest(node)->getNode_content().error > max_node->getNode_content().error)
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

	//Creamos los cotenidos basado en las posiciones e inicializamos los errores a 0
	NodeContent c1 = {{pos1[0], pos1[1]}, 0};
	NodeContent c2 = {{pos2[0], pos2[1]}, 0};

	//Insertamos los nodos al grafo
	base_graph.insert_node(c1);
	base_graph.insert_node(c2);

	//Insertamos una coneccion basica con edad 0
	base_graph.insert_edge(0, c1, c2, false);
}
template <class GNGTrait>
bool GNGAlgorithm<GNGTrait>::hay_coneccion(NodePtr node1, NodePtr node2){
	for(EdgePtr e : node1->getEdges_list()){
		if(e->getDest(node1) == node2)
			return true;
	}
	return false;
}
template<class GNGTrait>
double GNGAlgorithm<GNGTrait>::calcular_distancia(NodePtr node, sf::Vertex input){
	return sqrt(
			pow(abs(
					node->getNode_content().pos[0] - input.position.x
			), 2) +
			pow(abs(
					node->getNode_content().pos[1] - input.position.y
				), 2
			)
			);
}
template<class GNGTrait>
void GNGAlgorithm<GNGTrait>::exec(sf::Vertex input){
	NodePtr smallestNodes[2];
	smallestNodes[0] = nullptr;
	smallestNodes[1] = nullptr;

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos
	for(NodePtr node : base_graph.get_nodesVector()){
		if(smallestNodes[0] == nullptr) {
			smallestNodes[0] = node;
			continue;
		}
		else if(smallestNodes[1] == nullptr) {
			smallestNodes[1] = node;
			continue;
		}
		double distancia = calcular_distancia(node, input);
		if(distancia < calcular_distancia(smallestNodes[0], input))
			smallestNodes[0] = node;
		else if(distancia < calcular_distancia(smallestNodes[1], input))
			smallestNodes[1] = node;
	}

	assert(smallestNodes[0] != nullptr && smallestNodes[1] != nullptr);


	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante E_W multiplicado por el vector de diferencia entre
	//Input y el menor nodo

	double nueva_posicion[2] = {
			E_W *(input.position.x - smallestNodes[0]->getNode_content().pos[0]),
			E_W * (input.position.y - smallestNodes[0]->getNode_content().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	NodeContent c1 = {{nueva_posicion[0], nueva_posicion[1]},
				   smallestNodes[0]->getNode_content().error + calcular_distancia(smallestNodes[0], input)};

	smallestNodes[0]->setNode_content(c1);


	for(EdgePtr e : smallestNodes[0]->getEdges_list()){
		NodePtr node_destino = e->getDest(smallestNodes[0]);

		//Eliminamos la arista si exede el maximo age
		if(e->getEdge_content() > MAX_AGE) {
			base_graph.delete_edge(smallestNodes[0]->getNode_content(),
							  e->getDest(smallestNodes[0])->getNode_content(),
							  e->getEdge_content());
		}

		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante E_N
		double nueva_posicion_vecino[2] = {
				 E_N * (input.position.x - node_destino->getNode_content().pos[0]),
				 E_N * (input.position.y - node_destino->getNode_content().pos[1])
		};

		NodeContent nc_vecino = {{nueva_posicion_vecino[0], nueva_posicion_vecino[1]},
						   smallestNodes[0]->getNode_content().error};

		node_destino->setNode_content(nc_vecino);


		//Aumentamos el age en alguna cantidad ????
		e->setEdge_content(e->getEdge_content()+1); //TODO

		//Busca si hay conecion entre los nodos menores y si la hay
		//cambia su age a 0 sino crea el edge
		if(e->getDest(smallestNodes[0])){
			e->setEdge_content(0);
		}
	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!hay_coneccion(smallestNodes[0], smallestNodes[1]))
		base_graph.insert_edge(0, smallestNodes[0]->getNode_content(), smallestNodes[1]->getNode_content(), false);

	//Busca a lo largo de todos los nodos y si ya no tienen aristas los elimina del grafo
	for(NodePtr node : base_graph.get_nodesVector()){
		if(node->getEdges_list().empty())
			base_graph.delete_node(node->getNode_content());
	}

	if(SIGMA % iteracion == 0 && base_graph.size() < MAX_NODES) {
		NodePtr max_error_node = find_max_error(base_graph);
		NodePtr max_error_neighbor = find_max_error_connection(max_error_node);

		if (max_error_neighbor == nullptr)
			max_error_neighbor = max_error_node;


		//Creamps el contenido de un nuevo nodo y asignamos sus posiciones a la media
		//De las posiciones de los nodos con max_error y su max_error_neighbor

		NodeContent content_max = max_error_node->getNode_content();
		NodeContent content_neighbor = max_error_neighbor->getNode_content();

		double pos_modificada[2] = {
				content_max.pos[0] + content_neighbor.pos[0] / 2,
				content_neighbor.pos[1] + content_max.pos[1] / 2
		};
		NodeContent content = {{pos_modificada[0], pos_modificada[1]},
				content_max.error
		};

		base_graph.insert_node(content);

		//Creamos una arista entre max y su maximo vecino y el nuevo nodo
		//Askmismo eliminamos la arista entre el maximo y su vecino
		base_graph.insert_edge(0, content_max, content, false);
		base_graph.insert_edge(0, content_neighbor, content, false);

		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges_list()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		base_graph.delete_edge(content_max, content_neighbor,
						  edgePtr->getEdge_content());

		//Decrementamos el error de ambos nodos por ALFA y los asignamos
		//Asimismo decrementamos el erro de todos los nodos de la forma
		// error_x <- error_x - BETA * error_x

		content_max.error = content_max.error * ALFA;
		content_neighbor.error = content_neighbor.error * ALFA;

		max_error_node->setNode_content(content_max);
		max_error_neighbor->setNode_content(content_neighbor);

		for(NodePtr node_tmp : base_graph.get_nodesVector()){
			NodeContent content_tmp = node_tmp->getNode_content();
			content_tmp.error = content_tmp.error -  BETA * content_tmp.error;
			node_tmp->setNode_content(content_tmp);
		}
	}
	iteracion++;
}

template <class GNGTrait>
InputGenerator<GNGTrait>::InputGenerator(sf::VertexArray vertexArray){
        for(int x = 0;x < vertexArray.getVertexCount();x++){
        	sf::Vertex v = vertexArray[x];
        	pos_vector.push_back(v);
        }
}
template <class GNGTrait>
sf::Vertex InputGenerator<GNGTrait>::pop(){
	sf::Vertex toReturn = pos_vector.back();
	pos_vector.pop_back();
	return toReturn;
}
template<class GNGTrait>
unsigned long InputGenerator<GNGTrait>::size(){
	return pos_vector.size();
}
template<class GNGTrait>
void GNGContainer<GNGTrait>::draw_node(NodePtr node){
	sf::CircleShape node_circle(5 , 30);
	node_circle.setFillColor(sf::Color::Green);
	node_circle.setPosition(node->getNode_content().pos[0], node->getNode_content().pos[1]);
	nodes.push_back(node_circle);
}
template <class GNGTrait>
void GNGContainer<GNGTrait>::draw_edge(NodePtr start, NodePtr end){
	sf::Vector2f start_v(start->getNode_content().pos[0], start->getNode_content().pos[1]);
	sf::Vector2f end_v(end->getNode_content().pos[0], end->getNode_content().pos[1]);
	sf::Vertex edge[] = {
			sf::Vertex(start_v, sf::Color::Red),
			sf::Vertex(end_v, sf::Color::Red)
	};
	edges.push_back(edge);
}
template<class GNGTrait>
void GNGContainer<GNGTrait>::init(){
	window.create(sf::VideoMode(SCREEN_HEIGHT, SCREEN_WIDTH), "Growing Neural Gas");
	algo.init();
}

template<class GNGTrait>
void GNGContainer<GNGTrait>::start() {

	sf::VertexArray figure(sf::LinesStrip, 100);
	//Ciclo de ejecucion del programa
	while (window.isOpen()) {
		sf::Event event;

		//Ciclo de
		if (is_running) {
			InputGenerator<GNGTrait>inpt_gen = InputGenerator<GNGTrait>(figure);
			while(inpt_gen.size() > 0 && algo.get_iteracion() < 100000){
				sf::Vertex tmp = inpt_gen.pop();

				algo.exec(tmp);

				Graph<GNGTrait> g = algo.get_graph();

				for(NodePtr node_ptr : g.get_nodesVector()){
					sf::CircleShape node_circle(5 , 30);
					node_circle.setFillColor(sf::Color::Green);
					node_circle.setPosition(node_ptr->getNode_content().pos[0], node_ptr->getNode_content().pos[1]);
					//nodes.push_back(node_circle);

					window.draw(node_circle);

					for(EdgePtr edge_ptr : node_ptr->getEdges_list()){
						NodePtr node_dest_ptr = edge_ptr->getDest(node_ptr);

						sf::Vector2f start_v(node_ptr->getNode_content().pos[0], node_ptr->getNode_content().pos[1]);
						sf::Vector2f end_v(node_dest_ptr->getNode_content().pos[0], node_dest_ptr->getNode_content().pos[1]);
						sf::Vertex edge[] = {
								sf::Vertex(start_v, sf::Color::Red),
								sf::Vertex(end_v, sf::Color::Red)
						};
						window.draw(edge, 2, sf::LinesStrip);
						//draw_edge(node_ptr, edge_ptr->getDest(node_ptr));

						sf::CircleShape node_circle_dest(5 , 30);
						node_circle_dest.setFillColor(sf::Color::Green);
						node_circle_dest.setPosition(node_dest_ptr->getNode_content().pos[0], node_dest_ptr->getNode_content().pos[1]);
						window.draw(node_circle_dest);
					}
				}

				/*for (sf::CircleShape n : nodes) {
					window.draw(n);
				}
				for (sf::Vertex* vertexPtr : edges) {
					window.draw(vertexPtr, 2, sf::LinesStrip);
				}*/
				window.display();
			}
		}

		//Ciclo de eventos
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if(event.type == sf::Event::MouseMoved){
				if(is_running)
					continue;
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					sf::Vector2i pos = sf::Mouse::getPosition(window);
					sf::Vector2f pos_f = sf::Vector2f(pos.x, pos.y);
					figure.append(sf::Vertex(pos_f));
					// sf::CircleShape node(1);
					// node.setFillColor(sf::Color::White);
					// node.setPosition(pos.x, pos.y);

					// to_make.push_back(node);
				}

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if(is_running)
					is_running = false;
				else if(!is_running)
					is_running = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				if(is_running)
					continue;
				figure.clear();
				window.clear();
			}
		}


		window.draw(figure);
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