#include "gng_algo.h"

template<class Trait>
void GNGAlgorithm<Trait>::logMeanError(double sum){
    if(this->iteracion % this->mean_error_check == 0){
        cout << this->mean_error_sum / this->mean_error_check << " " << this->iteracion << endl;
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

    cout << this->ALFA << "," << this->BETA << "," << this->E_W
        << "," << this->E_N << "," << this->SIGMA << "," << this->MAX_NODES << "," << this->MAX_AGE << endl;  
}
template<class Trait>
void GNGAlgorithm<Trait>::exec(pair<int, int> input){
	NodePtr winner_nodes[2] = {nullptr, nullptr};

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto a los nodos
	
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(winner_nodes[0] == nullptr)
			winner_nodes[0] = node;
		else if(this->getDistance(node, input) < this->getDistance(winner_nodes[0], input)){
			winner_nodes[1] = winner_nodes[0];
			winner_nodes[0] = node;
		}
		else if(winner_nodes[1] == nullptr)
			winner_nodes[1] = node;

		else if(node < winner_nodes[1]  && node != winner_nodes[0])
			winner_nodes[1] = node;
	}	
	assert(winner_nodes[0] != nullptr && winner_nodes[1] != nullptr && winner_nodes[0] != winner_nodes[1]);

    this->logMeanError(sqrt(this->getDistance(winner_nodes[0], input)));

	//Mueve el nodo hacia el input en Ew y actualiza el error

    NodeContent updated_content = {{winner_nodes[0]->getContent().pos[0], winner_nodes[0]->getContent().pos[1]}, winner_nodes[0]->getContent().error};
    updated_content.error = updated_content.error + this->getDistance(winner_nodes[0], input);
    updated_content.pos[0] = updated_content.pos[0] + this->E_W * (input.first - updated_content.pos[0]);
    updated_content.pos[1] = updated_content.pos[1] + this->E_W * (input.second - updated_content.pos[1]);
	winner_nodes[0]->setContent(updated_content);


    //Update all topological neighbors of S by En
	for(EdgePtr e : winner_nodes[0]->getEdges()){
		NodePtr node_destino = e->getDest(winner_nodes[0]);
	

        NodeContent updated_ne = {{node_destino->getContent().pos[0], node_destino->getContent().pos[1]}, node_destino->getContent().error};
        updated_ne.pos[0] = updated_ne.pos[0] + this->E_N * (input.first - updated_ne.pos[0]);
        updated_ne.pos[1] = updated_ne.pos[1] + this->E_N * (input.second - updated_ne.pos[1]);
		node_destino->setContent(updated_ne);
        

        if(node_destino == winner_nodes[1])
            e->setContent(0);
		e->setContent(e->getContent()+1); 
	}
    
	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(winner_nodes[0], winner_nodes[1]))
		this->base_graph.insertEdge(0, winner_nodes[0]->getContent(), winner_nodes[1]->getContent(), false);

    //Quita los nodos con + de MAX_AGE
    for(NodePtr n : this->base_graph.getNodesVector()){
        for(EdgePtr e : n->getEdges()){
            if(e->getContent() >= this->MAX_AGE)
                this->base_graph.deleteEdge(n->getContent(), e->getDest(n)->getContent(), e->getContent());
        }
    }
	//Busca a lo largo de todos los nodos y si ya no tienen aristas los elimina del grafo
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(node->getEdges().empty())
			this->base_graph.deleteNode(node->getContent());
	}
	

	if(this->iteracion % this->SIGMA == 0 && this->base_graph.size() < this->MAX_NODES) {
		NodePtr max_error_node = this->findMaxError(this->base_graph);
		NodePtr max_error_neighbor = this->findMaxErrorLink(max_error_node);
		assert(max_error_neighbor != nullptr);
			
		NodeContent content_max = max_error_node->getContent();
		NodeContent content_neighbor = max_error_neighbor->getContent();

        NodeContent new_content = {{0, 0}, 0};
        new_content.pos[0] = (content_max.pos[0] + content_neighbor.pos[0]) / 2;
        new_content.pos[1] = (content_max.pos[1] + content_neighbor.pos[1]) / 2;

        //////////////////////////// Borra de manera mas simple /////////////
		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		this->base_graph.deleteEdge(content_max, content_neighbor,
						  edgePtr->getContent());

        ///////////////////////////////////////////////////////////
        
		content_neighbor.error = content_neighbor.error * this->ALFA;
		content_max.error = content_max.error * this->ALFA;
        new_content.error = content_max.error * this->ALFA;

		this->base_graph.insertNode(new_content);


		max_error_node->setContent(content_max);
		max_error_neighbor->setContent(content_neighbor);
        this->base_graph.insertNode(new_content);

		this->base_graph.insertEdge(0, content_max, new_content, false);
		this->base_graph.insertEdge(0, content_neighbor, new_content, false);
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
	NodePtr winner_nodes[2] = {nullptr, nullptr};

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto a los nodos
	
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(winner_nodes[0] == nullptr)
			winner_nodes[0] = node;
		else if(this->getDistance(node, input) < this->getDistance(winner_nodes[0], input)){
			winner_nodes[1] = winner_nodes[0];
			winner_nodes[0] = node;
		}
		else if(winner_nodes[1] == nullptr)
			winner_nodes[1] = node;

		else if(node < winner_nodes[1]  && node != winner_nodes[0])
			winner_nodes[1] = node;
	}	
	assert(winner_nodes[0] != nullptr && winner_nodes[1] != nullptr && winner_nodes[0] != winner_nodes[1]);

    this->logMeanError(sqrt(this->getDistance(winner_nodes[0], input)));

	//Mueve el nodo hacia el input en Ew y actualiza el error

    NodeContent updated_content = {{winner_nodes[0]->getContent().pos[0], winner_nodes[0]->getContent().pos[1]}, winner_nodes[0]->getContent().error, winner_nodes[0]->getContent().U};
    updated_content.pos[0] = updated_content.pos[0] + this->E_W * (input.first - updated_content.pos[0]);
    updated_content.pos[1] = updated_content.pos[1] + this->E_W * (input.second - updated_content.pos[1]);
    updated_content.U = updated_content.U + winner_nodes[1]->getContent().error - updated_content.error ;
    updated_content.error = updated_content.error + this->getDistance(winner_nodes[0], input);
	winner_nodes[0]->setContent(updated_content);


    //Update all topological neighbors of S by En
	for(EdgePtr e : winner_nodes[0]->getEdges()){
		NodePtr node_destino = e->getDest(winner_nodes[0]);
	

        NodeContent updated_ne = {{node_destino->getContent().pos[0], node_destino->getContent().pos[1]}, 
            node_destino->getContent().error, node_destino->getContent().U};
        updated_ne.pos[0] = updated_ne.pos[0] + this->E_N * (input.first - updated_ne.pos[0]);
        updated_ne.pos[1] = updated_ne.pos[1] + this->E_N * (input.second - updated_ne.pos[1]);
		node_destino->setContent(updated_ne);
        

        if(node_destino == winner_nodes[1])
            e->setContent(0);
		e->setContent(e->getContent()+1); 
	}
    
	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(winner_nodes[0], winner_nodes[1]))
		this->base_graph.insertEdge(0, winner_nodes[0]->getContent(), winner_nodes[1]->getContent(), false);

    //Quita los nodos con + de MAX_AGE
    for(NodePtr n : this->base_graph.getNodesVector()){
        for(EdgePtr e : n->getEdges()){
            if(e->getContent() >= this->MAX_AGE)
                this->base_graph.deleteEdge(n->getContent(), e->getDest(n)->getContent(), e->getContent());
        }
    }
	NodePtr max_error_node = this->findMaxError(this->base_graph);
    NodePtr min_util = this->findMinUtility(this->base_graph);
    
    double utility_meassure = (max_error_node->getContent().error / min_util->getContent().U);
    if(utility_meassure > this->K && min_util->getContent().U != 0){
        this->base_graph.deleteNode(min_util->getContent());
    }

	if(this->iteracion % this->SIGMA == 0 && this->base_graph.size() < this->MAX_NODES) {
		NodePtr max_error_node = this->findMaxError(this->base_graph);
		NodePtr max_error_neighbor = this->findMaxErrorLink(max_error_node);
		assert(max_error_neighbor != nullptr);
			
		NodeContent content_max = max_error_node->getContent();
		NodeContent content_neighbor = max_error_neighbor->getContent();

        NodeContent new_content = {{0, 0}, 0};
        new_content.pos[0] = (content_max.pos[0] + content_neighbor.pos[0]) / 2;
        new_content.pos[1] = (content_max.pos[1] + content_neighbor.pos[1]) / 2;
        new_content.U = (content_max.U + content_neighbor.U) / 2;

        //////////////////////////// Borra de manera mas simple /////////////
		EdgePtr edgePtr = nullptr;

		for(EdgePtr edge : max_error_node->getEdges()){
			if(edge->getDest(max_error_node) == max_error_neighbor)
				edgePtr = edge;
		}

		this->base_graph.deleteEdge(content_max, content_neighbor,
						  edgePtr->getContent());

        ///////////////////////////////////////////////////////////
        
		content_neighbor.error = content_neighbor.error * this->ALFA;
		content_max.error = content_max.error * this->ALFA;
        new_content.error = content_max.error;

		this->base_graph.insertNode(new_content);


		max_error_node->setContent(content_max);
		max_error_neighbor->setContent(content_neighbor);
        this->base_graph.insertNode(new_content);

		this->base_graph.insertEdge(0, content_max, new_content, false);
		this->base_graph.insertEdge(0, content_neighbor, new_content, false);
	}

	for(NodePtr node_tmp : this->base_graph.getNodesVector()){
			NodeContent content_tmp = node_tmp->getContent();
			content_tmp.error = content_tmp.error -  this->BETA * content_tmp.error;
            //content_tmp.U = content_tmp.U - this->BETA * content_tmp.U;
			node_tmp->setContent(content_tmp);
	}
	this->iteracion++;

}
template<class Trait>
void UGNGAlgorithm<Trait>::init() {
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

    cout << this->K << "," << this->ALFA << "," << this->BETA << "," << this->E_W
        << "," << this->E_N << "," << this->SIGMA << "," << this->MAX_NODES << "," << this->MAX_AGE << endl;  
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

