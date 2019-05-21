#include "gng_algo.h"
#include <ctime>
#include <cstdlib>

template <class Trait>
double GNGAlgorithm<Trait>::findMaxMeanError() const {
    double max_err = 0;
    for(NodePtr node : base_graph.getNodesVector()){
        if(node->getContent().error > max_err) 
            max_err = node->getContent().error;
    }
    return max_err;
}
template <class Trait>
int GNGAlgorithm<Trait>::getMaxAge() const {
    double max_age = 0;
    for(NodePtr node : this->base_graph.getNodesVector() ){
        for(EdgePtr edge : node->getEdges()){
            if(edge->getContent() > max_age)
                max_age = edge->getContent();
        }
    }
    return max_age;
}

template<class Trait>
auto GNGAlgorithm<Trait>::findMaxError(Graph<Trait> &graph) const{
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
double GNGAlgorithm<Trait>::getDistance(NodePtr node, pair<int, int> input) const {
    return pow(node->getContent().pos[0] - input.first, 2)  + 
            pow(node->getContent().pos[1] - input.second, 2);
        
}
template<class Trait>
double GNGAlgorithm<Trait>::getDistancePoint(pair<int, int> point, pair<int, int> input) const {
    return pow(point.first - input.first, 2) + pow(point.second - input.second, 2);
}
template<class Trait>
auto GNGAlgorithm<Trait>::findMaxErrorLink(NodePtr node) const {
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
void GNGAlgorithm<Trait>::init() {
	this->base_graph.clear();
	int pos1[2];
	int pos2[2];
    //TODO CLEANNN ^
    //             |  

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
	pair<NodePtr, NodePtr> smallestNodes;
	smallestNodes.first = nullptr;
	smallestNodes.second = nullptr;

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos
	
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(smallestNodes.first == nullptr)
			smallestNodes.first = node;
		else if(this->getDistance(node, input) < this->getDistance(smallestNodes.first, input)){
			smallestNodes.second = smallestNodes.first;
			smallestNodes.first = node;
		}
		else if(smallestNodes.second == nullptr)
			smallestNodes.second = node;

		else if(node < smallestNodes.second  && node != smallestNodes.first)
			smallestNodes.second = node;
	}	
	assert(smallestNodes.first != nullptr && smallestNodes.second != nullptr && smallestNodes.first != smallestNodes.second);


    this->logMeanError(sqrt(this->getDistance(smallestNodes.first, input))) ;
	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante this->E_W multiplicado por el vector de diferencia entre
	//Input y el menor nodo

    pair<int, int> new_pos = {
		smallestNodes.first->getContent().pos[0] + this->E_W *(input.first - smallestNodes.first->getContent().pos[0]),
		smallestNodes.first->getContent().pos[1] + this->E_W * (input.second - smallestNodes.first->getContent().pos[1])
	};

	//Actualizamos el valor del erro incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	NodeContent c1 = {{new_pos.first, new_pos.second},
				   smallestNodes.first->getContent().error + this->getDistance(smallestNodes.first, input)};

	smallestNodes.first->setContent(c1);


	for(EdgePtr e : smallestNodes.first->getEdges()){
		NodePtr node_destino = e->getDest(smallestNodes.first);
		
		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante this->E_N
		pair<int, int> new_pos_ne = {
            node_destino->getContent().pos[0] +  this->E_N * (input.first - node_destino->getContent().pos[0]),
            node_destino->getContent().pos[1] +  this->E_N * (input.second - node_destino->getContent().pos[1])
		};

		NodeContent nc_vecino = {{new_pos_ne.first, new_pos_ne.second},
						   node_destino->getContent().error};

		node_destino->setContent(nc_vecino);
		e->setContent(e->getContent()+1); 

        if(node_destino == smallestNodes.second)
            e->setContent(0);

		//Eliminamos la arista si exede el maximo age
		if(e->getContent() > this->MAX_AGE) {
			this->base_graph.deleteEdge(smallestNodes.first->getContent(),
							  e->getDest(smallestNodes.first)->getContent(),
							  e->getContent());
		}
	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(smallestNodes.first, smallestNodes.second))
		this->base_graph.insertEdge(0, smallestNodes.first->getContent(), smallestNodes.second->getContent(), false);

	//Busca a lo largo de todos los nodos y si ya no tienen aristas los elimina del grafo
	for(NodePtr node : this->base_graph.getNodesVector()){
		if(node->getEdges().empty())
			this->base_graph.deleteNode(node->getContent());
	}
	
    //cout << this->SIGMA << " " << this->iteracion << endl << endl;	
	if(this->iteracion % this->SIGMA == 0 && this->base_graph.size() < this->MAX_NODES) {
		NodePtr max_error_node = this->findMaxError(this->base_graph);
		NodePtr max_error_neighbor = this->findMaxErrorLink(max_error_node);


		assert(max_error_neighbor != nullptr);
			


		//Creamps el contenido de un nuevo nodo y asignamos sus posiciones a la media
		//De las posiciones de los nodos con max_error y su max_error_neighbor

		NodeContent content_max = max_error_node->getContent();
		NodeContent content_neighbor = max_error_neighbor->getContent();

        
	   pair<int, int> modified_pos = {
            (content_max.pos[0] + content_neighbor.pos[0]) / 2,
            (content_neighbor.pos[1] + content_max.pos[1]) / 2
		};
		NodeContent content = {{modified_pos.first, modified_pos.second},
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
	pair<NodePtr, NodePtr> smallestNodes = {nullptr, nullptr};

	//Seleccionamos los dos nodos con menor distancia euclidiana respecto
	//al nodo input y los guardamos

    for(NodePtr node : this->base_graph.getNodesVector()){
        double node_distance = this->getDistance(node, input);
        if(smallestNodes.first == nullptr || node_distance < this->getDistance(smallestNodes.first, input)){
            smallestNodes.second = smallestNodes.first;
            smallestNodes.first = node;
        }
        else if(smallestNodes.second == nullptr ||
                (node_distance < this->getDistance(smallestNodes.second, input) && node != smallestNodes.first)){
            smallestNodes.second = node;
        }
    }

	assert(smallestNodes.first != nullptr && smallestNodes.second != nullptr && smallestNodes.first != smallestNodes.second);


    //Mean error debuggin
    this->logMeanError(sqrt(this->getDistance(smallestNodes.first, input)));


	//Nueva posicion de el menor nodo
	//Esta posicion se calcula por una costante E_W multiplicado por el vector de diferencia entre
	//Input y el menor noo

    pair<int, int> new_pos = {
			smallestNodes.first->getContent().pos[0] + this->E_W *(input.first - smallestNodes.first->getContent().pos[0]),
			smallestNodes.first->getContent().pos[1] + this->E_W * (input.second - smallestNodes.first->getContent().pos[1])
    };

	//Actualizamos el valor error local incrementantodolo por la distancia
	//y insertamos la nueva posicion del nodo
	double n_erro = smallestNodes.first->getContent().error + this->getDistance(smallestNodes.first, input);
    cout << "N_erro: " << n_erro << endl;
	NodeContent c1 = {{new_pos.first,new_pos.second},
				        n_erro,
                        smallestNodes.first->getContent().U + smallestNodes.second->getContent().error - smallestNodes.first->getContent().error 
       	            }; 

	smallestNodes.first->setContent(c1);

	
	for(EdgePtr e : smallestNodes.first->getEdges()){
		NodePtr node_dest = e->getDest(smallestNodes.first);
		

		//Movemos los nodos vecinos acercandose al input
		//En el vector de diferencia entre el nodo vecino e input multiplicado por una constante this->E_N
        pair<int, int> new_pos_ne{
            node_dest->getContent().pos[0] +  this->E_N * (input.first - node_dest->getContent().pos[0]),
            node_dest->getContent().pos[1] +  this->E_N * (input.second - node_dest->getContent().pos[1])
        };
       
		NodeContent nc_vecino = {{new_pos_ne.first, new_pos_ne.second},
						   node_dest->getContent().error,
						   node_dest->getContent().U};

		node_dest->setContent(nc_vecino);

		e->setContent(e->getContent()+1); 

        if(node_dest == smallestNodes.second)
            e->setContent(0);

		//Eliminamos la arista si exede el maximo age
		if(e->getContent() > this->MAX_AGE) {
			this->base_graph.deleteEdge(smallestNodes.first->getContent(),
							  e->getDest(smallestNodes.first)->getContent(),
							  e->getContent());
		}
	}

	//Se crea una coneccion entre los nodos menores si no hay una
	if(!this->isConnected(smallestNodes.first, smallestNodes.second))
		this->base_graph.insertEdge(0, smallestNodes.first->getContent(), smallestNodes.second->getContent(), false);
	
    //Calcula la utilidad del nodo y lo eliminaa dependiendo de que tan baja es
    NodePtr max_error_node = this->findMaxError(this->base_graph);
    NodePtr min_util = this->findMinUtility(this->base_graph);

    double util_meassure = max_error_node->getContent().error / min_util->getContent().U;
    cout << "Utility: " << min_util->getContent().U << endl;
    cout << "Max error: " << max_error_node->getContent().error << endl;
    cout << "Utility meassure of lowest utility node: " << util_meassure << endl << endl;
	if(util_meassure > this->K && min_util != 0){
		this->base_graph.deleteNode(min_util->getContent());
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

        pair<int, int> modified_pos = {
            (content_max.pos[0] + content_neighbor.pos[0]) / 2,
            (content_neighbor.pos[1] + content_max.pos[1]) / 2
        };

		NodeContent content = {{modified_pos.first, modified_pos.second},
				content_max.error * this->ALFA, (content_max.U + content_neighbor.U) / 2
		};

		this->base_graph.insertNode(content);

		//Creamos una arista entre max y su maximo vecino y el nuevo nodo
		//Asimismo eliminamos la arista entre el maximo y su vecino
		this->base_graph.insertEdge(0, content_max, content, false);
		this->base_graph.insertEdge(0, content_neighbor, content, false);
        this->base_graph.deleteEdge(content_max, content_neighbor);


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
    pair<int, int> point_1, point_2;
    double error_1, error_2;

    srand(time(0));
	this->base_graph.clear();
    
	//Creamos dos puntos al azar en el espacio de la pantalla
	point_1.first = rand() % this->SCREEN_HEIGHT;
	point_1.second = rand() % this->SCREEN_WIDTH;
	point_2.first = rand() % this->SCREEN_HEIGHT;
	point_2.second = rand() % this->SCREEN_WIDTH;

	//Creamos los contenidos basado en las posiciones e inicializamos los errores y la utilidad
	NodeContent c1 = {{point_1.first, point_1.second}, 0, 0};
	NodeContent c2 = {{point_2.first, point_2.second}, 0, 0};

	//Insertamos los nodos al grafo
	this->base_graph.insertNode(c1);
	this->base_graph.insertNode(c2);

	//Insertamos una coneccion basica con edad 0
	this->base_graph.insertEdge(0, c1, c2, false);
}
template <class Trait>
bool GNGAlgorithm<Trait>::isConnected(NodePtr node1, NodePtr node2) const{
	for(EdgePtr e : node1->getEdges()){
		if(e->getDest(node1) == node2)
			return true;
	}
	return false;
}

template<class Trait>
auto UGNGAlgorithm<Trait>::findMinUtility(Graph<Trait> &graph) const{
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

template<class Trait>
auto UGNGAlgorithm<Trait>::findMinUtilityNear(NodePtr node_ptr) const {
    NodePtr min_util = nullptr;
    for(auto edge : node_ptr->getEdges()){
        NodePtr node_dest = edge->getDest(node_ptr);

        if(min_util == nullptr){
            min_util = node_dest;
            continue;
        }

        if(node_dest->getContent().U < min_util->getContent().U)
            min_util = node_dest;
    }
    return min_util;
}

