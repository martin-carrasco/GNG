#ifndef GRAPH_NULL_GNG_ALGO_H
#define GRAPH_NULL_GNG_ALGO_H

#include "SFML/Graphics.hpp"
#include "../graph.h"

template <class GNGTrait>
class GNGAlgorithm{
	protected:
    typedef typename Graph<GNGTrait>::NodeContent NodeContent;
    typedef ::Edge<Graph<GNGTrait>>* EdgePtr;
    typedef ::Node<Graph<GNGTrait>>* NodePtr;

    static constexpr double K = 3; //Medida de utilidad para eliminar un nodo
    static constexpr int SIGMA = 400; //Mean error (Medida de error maxima aceptable)
    static constexpr double ALFA = 0.5;
    static constexpr double BETA =0.0005;
    static constexpr double E_W = 0.10; //Movimiento del nodo de comparacion respecto al input
    static constexpr double E_N = 0.0010; //Movimiento del nodo input respecto al nodo de comparacion
    static constexpr double MAX_AGE = 50; //TODO Cambiar ???
    static constexpr int MAX_NODES = 100; //TODO cambiar ???


		unsigned int SCREEN_HEIGHT;
		unsigned int SCREEN_WIDTH;
    
		unsigned int iteracion = 1;
    Graph<GNGTrait> base_graph;

   //Find the node with the highest error in the graph
    auto findMaxError(Graph<GNGTrait> &graph);
    
    //Finds the neighbor of node with the highest error value
    auto findMaxErrorLink(NodePtr node);

	public:
    GNGAlgorithm(unsigned int screen_height, unsigned int screen_width) : SCREEN_HEIGHT(screen_height), SCREEN_WIDTH(screen_width){}
    virtual void init();
    bool isConnected(NodePtr node1, NodePtr node2);
    double getDistance(NodePtr node, sf::Vertex input);
		virtual void exec(sf::Vertex input);
    int get_iteracion(){
        return iteracion;
    }
    auto get_graph(){
        return base_graph;
    }

};

template <class GNGTrait>
class PictureGNGAlgorithm : public GNGAlgorithm<GNGTrait> {
	private:
		typedef typename Graph<GNGTrait>::NodeContent NodeContent;
    typedef ::Edge<Graph<GNGTrait>>* EdgePtr;
    typedef ::Node<Graph<GNGTrait>>* NodePtr;


		int * pic_buffer;	
	public:
		PictureGNGAlgorithm(unsigned int screen_height, unsigned int screen_width);
		void load_picture_buffer(string location);
		virtual void init();
		virtual void exec(sf::Vertex);
};
template <class GNGTrait>
class UGNGAlgorithm : public  GNGAlgorithm<GNGTrait> {
	private:	
    typedef typename Graph<GNGTrait>::NodeContent NodeContent;
    typedef ::Edge<Graph<GNGTrait>>* EdgePtr;
    typedef ::Node<Graph<GNGTrait>>* NodePtr;

		 //Encuentra el nodo con la minima utilidad
		auto findMinUtility(Graph<GNGTrait> &graph);
	public:
		UGNGAlgorithm(unsigned int screen_height, unsigned int screen_width);
		virtual void init();
		virtual void exec(sf::Vertex);
};
#include "gng_algo.cpp"
#endif //GRAPH_NULL_GNG_H

