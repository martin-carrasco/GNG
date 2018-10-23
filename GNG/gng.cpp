#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "../graph.h"


#define SCREEN_X 300
#define SCREEN_Y 300

using namespace std;

template <class GNGTrait>
class InputGenerator;

template <class GNGTrait>
class GNGAlgorithm{
	typedef typename Graph<GNGTrait>::Node Node;
	typedef typename Graph<GNGTrait>::Edge Edge;
	typedef typename Graph<GNGTrait>::NodeContent NodeContent;
	typedef typename Graph<GNGTrait>::EdgeContent EdgeContent;
	typedef Edge* EdgePtr;
	typedef Node* NodePtr;

	static constexpr int SIGMA = 300; //Mean error (Medida de error maxima aceptable)
	static constexpr double ALFA = 0.5;
	static constexpr double BETA =0.0005;
	static constexpr double E_W = 0.05; //Movimiento del nodo de comparacion respecto al input
	static constexpr double E_N = 0.0006; //Movimiento del nodo input respecto al nodo de comparacion
	static constexpr double MAX_AGE = 1000; //TODO Cambiar ???
	static constexpr int MAX_NODES = 100; //TODO cambiar ???

	unsigned int iteracion = 0;
	Graph<GNGTrait> base_graph;
    //Find the node with the highest error in the graph
	NodePtr find_max_error(Graph<GNGTrait> &graph){
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
	NodePtr find_max_error_connection(NodePtr node){
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

public:
    void init() {
	    iteracion = 0;
		base_graph.clear();
	    int pos1[2];
		int pos2[2];

		//Creamos dos puntos al azar en el espacio de la pantalla
		pos1[0] = rand() % SCREEN_X;
		pos1[1] = rand() % SCREEN_Y;
		pos2[0] = rand() % SCREEN_X;
		pos2[1] = rand() % SCREEN_Y;

		//Creamos los cotenidos basado en las posiciones e inicializamos los errores a 0
		NodeContent c1 = {.pos[0] = pos1[0], .pos[1] = pos1[1], .error=0};
		NodeContent c2 = {.pos[0] = pos2[0], .pos[1] = pos2[1], .error=0};

		//Insertamos los nodos al grafo
		base_graph.insert_node(c1);
		base_graph.insert_node(c2);

		//Insertamos una coneccion basica con edad 0
		base_graph.insert_edge(0, c1, c2, false);
	}
	bool hay_coneccion(NodePtr node1, NodePtr node2){
	    for(EdgePtr e : node1->getEdges_list()){
	        if(e->getDest(node1) == node2)
	            return true;
	    }
	    return false;
	}
	double calcular_distancia(NodePtr node1, NodePtr node2){
	    return sqrt(
                pow(abs(
                        node1->getNode_content().pos[0] - node2->getNode_content().pos[0]
                ), 2) +
                pow(abs(
                        node1->getNode_content().pos[1] - node2->getNode_content().pos[1]
                    ), 2
                )
        );
	}
	void exec(NodePtr input){
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
        int nueva_posicion[2] = {
                (int) E_W *(input->getNode_content().pos[0] - smallestNodes[0]->getNode_content().pos[0]),
                (int) E_W * (input->getNode_content().pos[1] - smallestNodes[0]->getNode_content().pos[1])
        };

        //Actualizamos el valor del erro incrementantodolo por la distancia
        //y insertamos la nueva posicion del nodo
        NodeContent c1 = {.pos[0] = nueva_posicion[0],
                          .pos[1] = nueva_posicion[1],
                .error=smallestNodes[0]->getNode_content().error + calcular_distancia(smallestNodes[0], input)};

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
            int nueva_posicion_vecino[2] = {
                    (int) E_N * (input->getNode_content().pos[0] - node_destino->getNode_content().pos[0]),
                    (int) E_N * (input->getNode_content().pos[1] - node_destino->getNode_content().pos[1])
            };

            NodeContent nc_vecino = {.pos[0] = nueva_posicion_vecino[0],
                    .pos[1] = nueva_posicion_vecino[1],
                    .error=smallestNodes[0]->getNode_content().error};

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

            NodeContent content = {
                    .pos[0] = (content_max.pos[0] + content_neighbor.pos[0] / 2),
                    .pos[1] = (content_neighbor.pos[1] + content_max.pos[1] / 2),
                    .error=content_max.error
            };

            base_graph.insert_node(content);

            //Creamos una arista entre max y su maximo vecino y el nuevo nodo
            //Askmismo eliminamos la arista entre el maximo y su vecino
            base_graph.insert_edge(0, content_max, content, false);
            base_graph.insert_edge(0, content_neighbor, content, false);

            base_graph.delete_edge(content_max, content_neighbor,
                              base_graph.find_edge_between_nodes(content_max, content_neighbor));

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

};

template <class GNGTrait>
class InputGenerator{
	typedef typename Graph<GNGTrait>::Node Node;
	typedef typename Graph<GNGTrait>::Edge Edge;
	typedef Edge* EdgePtr;
	typedef Node* NodePtr;
    //Vectores de input que conforman la imagen
	vector<NodePtr> pos_vector;

public:
    InputGenerator(vector<NodePtr> pos_vector){
        this->pos_vector = pos_vector;
    }
    NodePtr pop(){
        NodePtr toReturn = pos_vector.back();
        pos_vector.pop_back();
        return toReturn;
    }
    unsigned long size(){
        return pos_vector.size();
    }
};
template<class GNGTrait>
class GNGContainer{
	typedef typename Graph<GNGTrait>::Node Node;
	typedef typename Graph<GNGTrait>::Edge Edge;
	typedef typename Graph<GNGTrait>::NodeContent NodeContent;
	typedef typename Graph<GNGTrait>::EdgeContent EdgeContent;
	typedef Edge* EdgePtr;
	typedef Node* NodePtr;
public:
	void init(vector<NodePtr> vector_pos){
	    //TODO Dibujar primera figura


	    //TODO criterio para hallar el input
        GNGAlgorithm<GNGTrait> algo;
        InputGenerator<GNGTrait> inputGenerator;

        algo.init();

        //Ejecutamox x iteraciones
        while(inputGenerator.size() > 0)
            algo.exec(inputGenerator.pop());

	}
};