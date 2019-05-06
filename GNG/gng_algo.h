#ifndef GRAPH_NULL_GNG_ALGO_H
#define GRAPH_NULL_GNG_ALGO_H

#include "SFML/Graphics.hpp"
#include "../graph.h"
#include <cmath>

template <class Trait>
class GNGAlgorithm {
protected:
    typedef typename Graph<Trait>::NodeContent NodeContent;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    typedef ::Node<Graph<Trait>>* NodePtr;

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

    Graph<Trait> base_graph; 
   
    //Finds the node with the max error
    auto findMaxError(Graph<Trait> &graph); 

    //Finds the neighbor with the max error of said node
    auto findMaxErrorLink(NodePtr node);

public:
    GNGAlgorithm(unsigned int screen_height, unsigned int screen_width) : SCREEN_HEIGHT(screen_height), SCREEN_WIDTH(screen_width) {}
    bool isConnected(NodePtr node1, NodePtr node2);
    double getDistance(NodePtr node, sf::Vertex input);
    int get_iteracion();
    auto get_graph(){
        return base_graph;
    }
    virtual void exec(sf::Vertex input); 
    virtual void init();
};

template <class Trait>
class DefaultGNGAlgorithm : public GNGAlgorithm<Trait> {
    typedef typename Graph<Trait>::NodeContent NodeContent;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    typedef ::Node<Graph<Trait>>* NodePtr;

public:
    DefaultGNGAlgorithm(unsigned int screen_height, unsigned int screen_width) : GNGAlgorithm<Trait>(screen_height, screen_width){}
    virtual void init();
	virtual void exec(sf::Vertex input);
    bool isConnected(NodePtr node1, NodePtr node2);
    double getDistance(NodePtr node, sf::Vertex input);

};

template <class Trait>
class UGNGAlgorithm : public GNGAlgorithm<Trait> { 
    typedef typename Graph<Trait>::NodeContent NodeContent;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    typedef ::Node<Graph<Trait>>* NodePtr;

    //Encuentra el nodo con la minima utilidad
	auto findMinUtility(Graph<Trait> &graph);

 public:
    UGNGAlgorithm(unsigned int screen_height, unsigned int screen_width) : GNGAlgorithm<Trait>(screen_height, screen_width){}
    virtual void init();
    virtual	void exec(sf::Vertex input);
};
#include "gng_algo.cpp"
#endif //GRAPH_NULL_GNG_H

