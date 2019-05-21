#include "gng_algo.h"

template<class Trait>
void GNGAlgorithm<Trait>::logMeanError(double sum){
    if(this->iteracion % this->mean_error_check == 0){
        cout << "Mean Error " << this->mean_error_sum / this->mean_error_check << " iteration " << this->iteracion << endl;
        this->mean_error_sum = 0;
    }else{
        this->mean_error_sum += sum;
    }
}

template <class Trait>
int GNGAlgorithm<Trait>::getMaxAge(){
    int max_age = 0;
    for(NodePtr node : this->base_graph.getNodesVector() ){
        for(EdgePtr edge : node->getEdges()){
            if(edge->getContent() > max_age)
                max_age = edge->getContent();
        }
    }
    return max_age;
}

template<class Trait>
auto GNGAlgorithm<Trait>::findMaxError(Graph<Trait> &graph){
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
template<class Trait>
double GNGAlgorithm<Trait>::getDistance(NodePtr node, pair<int, int> input){
    return
            pow(node->getContent().pos[0] - input.first, 2)  + 
            pow(node->getContent().pos[1] - input.second, 2);
        
}
template<class Trait>
auto GNGAlgorithm<Trait>::findMaxErrorLink(NodePtr node){
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
template <class Trait>
void GNGAlgorithm<Trait>::init() {
    //this->iteracion = 0;
	this->base_graph.clear();
	double pos1[2];
	double pos2[2];

	//Creamos dos puntos al azar en el espacio de la pantalla
	pos1[0] = rand() % this->SCREEN_HEIGHT;
	pos1[1] = rand() % this->SCREEN_WIDTH;
	pos2[0] = rand() % this->SCREEN_HEIGHT;
	pos2[1] = rand() % this->SCREEN_WIDTH;

	//Creamos los contenidos basado en las posiciones e inicializamos los errores a 0
	NodeContent c1 = {{pos1[0], pos1[1]}, 0};
	NodeContent c2 = {{pos2[0], pos2[1]}, 0};

	//Insertamos los nodos al grafo
	this->base_graph.insertNode(c1);
	this->base_graph.insertNode(c2);

	//Insertamos una coneccion basica con edad 0
	this->base_graph.insertEdge(0, c1, c2, false);

}
template<class Trait>
void GNGAlgorithm<Trait>::exec(pair<int, int> input){
	NodePtr smallestNodes[2];
	smallestNodes[0] = nullptr;
	smallestNodes[1] = nullptr;

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos
	
	double dist_small2_inpt;
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(smallestNodes[0] == nullptr)
			smallestNodes[0] = node;
		else if(this->getDistance(node, input) < this->getDistance(smallestNodes[0], input)){
			smallestNodes[1] = smallestNodes[0];
			smallestNodes[0] = node;
		}
		else if(smallestNodes[1] == nullptr)
			smallestNodes[1] = node;

		else if(node < smallestNodes[1]  && node != smallestNodes[0])
			smallestNodes[1] = node;
	}	
	assert(smallestNodes[0] != nullptr && smallestNodes[1] != nullptr && smallestNodes[0] != smallestNodes[1]);

    this->logMeanError(sqrt(this->getDistance(smallestNodes[0], input)));

	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante this->E_W multiplicado por el vector de diferencia entre
	//Input y el menor nodo

	double nueva_posicion[2] = {
			smallestNodes[0]->getContent().pos[0] + this->E_W *(input.first - smallestNodes[0]->getContent().pos[0]),
			smallestNodes[0]->getContent().pos[1] + this->E_W * (input.second - smallestNodes[0]->getContent().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	NodeContent c1 = {{nueva_posicion[0], nueva_posicion[1]},
				   smallestNodes[0]->getContent().error + this->getDistance(smallestNodes[0], input)};

	smallestNodes[0]->setContent(c1);


	for(EdgePtr e : smallestNodes[0]->getEdges()){
		NodePtr node_destino = e->getDest(smallestNodes[0]);
		
		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante this->E_N
		double nueva_posicion_vecino[2] = {
				node_destino->getContent().pos[0] +  this->E_N * (input.first - node_destino->getContent().pos[0]),
				node_destino->getContent().pos[1] +  this->E_N * (input.second - node_destino->getContent().pos[1])
		};

		NodeContent nc_vecino = {{nueva_posicion_vecino[0], nueva_posicion_vecino[1]},
						   node_destino->getContent().error};

		node_destino->setContent(nc_vecino);
		e->setContent(e->getContent()+1); 

        if(node_destino == smallestNodes[1])
            e->setContent(0);

		//Eliminamos la arista si exede el maximo age
		if(e->getContent() > this->MAX_AGE) {
			this->base_graph.deleteEdge(smallestNodes[0]->getContent(),
							  e->getDest(smallestNodes[0])->getContent(),
							  e->getContent());
		}
	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(smallestNodes[0], smallestNodes[1]))
		this->base_graph.insertEdge(0, smallestNodes[0]->getContent(), smallestNodes[1]->getContent(), false);

	//Busca a lo largo de todos los nodos y si ya no tienen aristas los elimina del grafo
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(node->getEdges().empty())
			this->base_graph.deleteNode(node->getContent());
	}
	
	if(this->iteracion % this->SIGMA == 0 && this->base_graph.size() < this->MAX_NODES) {
		NodePtr max_error_node = this->findMaxError(this->base_graph);
		NodePtr max_error_neighbor = this->findMaxErrorLink(max_error_node);


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
				content_max.error * this->ALFA
		};

		this->base_graph.insertNode(content);

		//Creamos una arista entre max y su maximo vecino y el nuevo nodo
		//Asimismo eliminamos la arista entre el maximo y su vecino
		this->base_graph.insertEdge(0, content_max, content, false);
		this->base_graph.insertEdge(0, content_neighbor, content, false);

		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		this->base_graph.deleteEdge(content_max, content_neighbor,
						  edgePtr->getContent());

		//Decrementamos el error de ambos nodos por this->ALFA y los asignamos
		//Asimismo decrementamos el erro de todos los nodos de la forma
		// error_x <- error_x - this->BETA * error_x

		content_max.error = content_max.error * this->ALFA;
		content_neighbor.error = content_neighbor.error * this->ALFA;

		max_error_node->setContent(content_max);
		max_error_neighbor->setContent(content_neighbor);

	}

	for(NodePtr node_tmp : this->base_graph.getNodesVector()){
			NodeContent content_tmp = node_tmp->getContent();
			content_tmp.error = content_tmp.error -  this->BETA * content_tmp.error;
			node_tmp->setContent(content_tmp);
	}
	this->iteracion++;

}


template<class Trait>
void UGNGAlgorithm<Trait>::exec(pair<int, int> input){
	NodePtr smallestNodes[2];
	smallestNodes[0] = nullptr;
	smallestNodes[1] = nullptr;

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos
	
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(smallestNodes[0] == nullptr)
			smallestNodes[0] = node;
		else if(this->getDistance(node, input) < this->getDistance(smallestNodes[0], input)){
			smallestNodes[1] = smallestNodes[0];
			smallestNodes[0] = node;
		}
		else if(smallestNodes[1] == nullptr)
			smallestNodes[1] = node;

		else if(node < smallestNodes[1]  && node != smallestNodes[0])
			smallestNodes[1] = node;
	}	
	assert(smallestNodes[0] != nullptr && smallestNodes[1] != nullptr && smallestNodes[0] != smallestNodes[1]);

    this->logMeanError(sqrt(this->getDistance(smallestNodes[0], input)));

	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante E_W multiplicado por el vector de diferencia entre
	//Input y el menor nodo

	double nueva_posicion[2] = {
			smallestNodes[0]->getContent().pos[0] + this->E_W *(input.first - smallestNodes[0]->getContent().pos[0]),
			smallestNodes[0]->getContent().pos[1] + this->E_W * (input.second - smallestNodes[0]->getContent().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	double n_erro = smallestNodes[0]->getContent().error + this->getDistance(smallestNodes[0], input);
	NodeContent c1 = {{nueva_posicion[0], nueva_posicion[1]},
				   n_erro,
       				smallestNodes[1]->getContent().error - n_erro //TODO MAKE SURE ITS NOT NEGATIVE!
				       	+ smallestNodes[0]->getContent().U }; 

	smallestNodes[0]->setContent(c1);

	
	for(EdgePtr e : smallestNodes[0]->getEdges()){
		NodePtr node_destino = e->getDest(smallestNodes[0]);
		

		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante this->E_N
		double nueva_posicion_vecino[2] = {
				node_destino->getContent().pos[0] +  this->E_N * (input.first - node_destino->getContent().pos[0]),
				node_destino->getContent().pos[1] +  this->E_N * (input.second - node_destino->getContent().pos[1])
		};

		NodeContent nc_vecino = {{nueva_posicion_vecino[0], nueva_posicion_vecino[1]},
						   node_destino->getContent().error,
						   node_destino->getContent().U};

		node_destino->setContent(nc_vecino);

		e->setContent(e->getContent()+1); 

        if(node_destino == smallestNodes[1])
            e->setContent(0);

		//Eliminamos la arista si exede el maximo age
		if(e->getContent() > this->MAX_AGE) {
			this->base_graph.deleteEdge(smallestNodes[0]->getContent(),
							  e->getDest(smallestNodes[0])->getContent(),
							  e->getContent());
		}

	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(smallestNodes[0], smallestNodes[1]))
		this->base_graph.insertEdge(0, smallestNodes[0]->getContent(), smallestNodes[1]->getContent(), false);

	//Busca el numero con la menor utilidad y divide el mayor error entre esta
	NodeContent min_utility_content = this->findMinUtility(this->base_graph)->getContent();
	NodePtr max_error_node = this->findMaxError(this->base_graph);


    double utility_meassure = max_error_node->getContent().error / min_utility_content.U;
    cout << "Utility: " << utility_meassure << endl;
    //Calcula la utilidad del nodo y lo eliminaa dependiendo de que tan baja es
	if(utility_meassure > this->K){	
		this->base_graph.deleteNode(min_utility_content);
	}
	
	
	if(this->iteracion % this->SIGMA == 0 && this->base_graph.size() < this->MAX_NODES) {
		NodePtr max_error_neighbor = this->findMaxErrorLink(max_error_node);
		
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
				content_max.error * this->ALFA, (content_max.U + content_neighbor.U) / 2
		};

		this->base_graph.insertNode(content);

		//Creamos una arista entre max y su maximo vecino y el nuevo nodo
		//Asimismo eliminamos la arista entre el maximo y su vecino
		this->base_graph.insertEdge(0, content_max, content, false);
		this->base_graph.insertEdge(0, content_neighbor, content, false);

		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		this->base_graph.deleteEdge(content_max, content_neighbor,
						  edgePtr->getContent());

		//Decrementamos el error de ambos nodos por this->ALFA y los asignamos
		//Asimismo decrementamos el erro de todos los nodos de la forma
		// error_x <- error_x - this->BETA * error_x

		content_max.error = content_max.error * this->ALFA;
		content_neighbor.error = content_neighbor.error * this->ALFA;

		max_error_node->setContent(content_max);
		max_error_neighbor->setContent(content_neighbor);

	}

    for(NodePtr node_tmp : this->base_graph.getNodesVector()){
        NodeContent content_tmp = node_tmp->getContent();
        content_tmp.error = content_tmp.error -  this->BETA * content_tmp.error;
        content_tmp.U = content_tmp.U - this->BETA * content_tmp.U;
        node_tmp->setContent(content_tmp);
    }
	this->iteracion++;

}
template<class Trait>
void UGNGAlgorithm<Trait>::init() {
	//this->iteracion = 0;
	this->base_graph.clear();
	double pos1[2];
	double pos2[2];

	//Creamos dos puntos al azar en el espacio de la pantalla
	pos1[0] = rand() % this->SCREEN_HEIGHT;
	pos1[1] = rand() % this->SCREEN_WIDTH;
	pos2[0] = rand() % this->SCREEN_HEIGHT;
	pos2[1] = rand() % this->SCREEN_WIDTH;

	//Creamos los contenidos basado en las posiciones e inicializamos los errores a 0
	NodeContent c1 = {{pos1[0], pos1[1]}, 0, 0};
	NodeContent c2 = {{pos2[0], pos2[1]}, 0, 0};

	//Insertamos los nodos al grafo
	this->base_graph.insertNode(c1);
	this->base_graph.insertNode(c2);

	//Insertamos una coneccion basica con edad 0
	this->base_graph.insertEdge(0, c1, c2, false);
}
template <class Trait>
bool GNGAlgorithm<Trait>::isConnected(NodePtr node1, NodePtr node2){
	for(EdgePtr e : node1->getEdges()){
		if(e->getDest(node1) == node2)
			return true;
	}
	return false;
}

template<class Trait>
auto UGNGAlgorithm<Trait>::findMinUtility(Graph<Trait> &graph){
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

