#include "gng_algo.h"

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
	pos1[0] = rand() % this->SCREEN_HEIGHT;
	pos1[1] = rand() % this->SCREEN_WIDTH;
	pos2[0] = rand() % this->SCREEN_HEIGHT;
	pos2[1] = rand() % this->CREEN_WIDTH;

	//Creamos los contenidos basado en las posiciones e inicializamos los errores a 0
	NodeContent c1 = {{pos1[0], pos1[1]}, 0, 0};
	NodeContent c2 = {{pos2[0], pos2[1]}, 0, 0};

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
	//Input y el menor noo

	double nueva_posicion[2] = {
			smallestNodes[0]->getContent().pos[0] + E_W *(input.position.x - smallestNodes[0]->getContent().pos[0]),
			smallestNodes[0]->getContent().pos[1] + E_W * (input.position.y - smallestNodes[0]->getContent().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	double n_erro = smallestNodes[0]->getContent().error + getDistance(smallestNodes[0], input);
	NodeContent c1 = {{nueva_posicion[0], nueva_posicion[1]},
				   n_erro,
       				smallestNodes[1]->getContent().error - n_erro
				       	+ smallestNodes[0]->getContent().U}; 

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
						   smallestNodes[0]->getContent().error,
						   smallestNodes[0]->getContent().U};

		node_destino->setContent(nc_vecino);

		//TODO Probar diferentes valores para AGE
		e->setContent(e->getContent()+1); 

	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!isConnected(smallestNodes[0], smallestNodes[1]))
		base_graph.insertEdge(0, smallestNodes[0]->getContent(), smallestNodes[1]->getContent(), false);

	//Busca el numero con la menor utilidad y divide el mayor error entre esta
	NodeContent min_utility_content = findMinUtility(base_graph)->getContent();
	NodeContent tmp_max_error = findMaxError(base_graph)->getContent();
	

	if((tmp_max_error.error / min_utility_content.U) > K){	
		base_graph.deleteNode(min_utility_content);
	}
	
	
        //cout << SIGMA << " " << iteracion << endl << endl;	
	if(iteracion % SIGMA == 0 && base_graph.size() < MAX_NODES) {
		NodePtr max_error_node = findMaxError(base_graph);
		NodePtr max_error_neighbor = findMaxErrorLink(max_error_node);
		
		if(max_error_neighbor == nullptr)
			return;
		//assert(max_error_neighbor != nullptr);
			


		//Creamps el contenido de un nuevo nodo y asignamos sus posiciones a la media
		//De las posiciones de los nodos con max_error y su max_error_neighbor

		NodeContent content_max = max_error_node->getContent();
		NodeContent content_neighbor = max_error_neighbor->getContent();

		double pos_modificada[2] = {
				(content_max.pos[0] + content_neighbor.pos[0]) / 2,
				(content_neighbor.pos[1] + content_max.pos[1]) / 2
		};
		NodeContent content = {{pos_modificada[0], pos_modificada[1]},
				content_max.error, (content_max.U + content_neighbor.U) / 2
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
			content_tmp.U = content_tmp.U - BETA * content_tmp.U;
			node_tmp->setContent(content_tmp);
		}
	}
	iteracion++;

}

template<class GNGTrait>
auto UGNGAlgorithm<GNGTrait>::findMinUtility(Graph<GNGTrait> &graph){
	NodePtr min_node = nullptr;
	for(NodePtr node : graph.getNodesVector()){
		if(min_node == nullptr)
			min_node = node;
		else
			if(node->getContent().U < min_node->getContent().U)
				min_node = node;
	}
	return min_node;
}


