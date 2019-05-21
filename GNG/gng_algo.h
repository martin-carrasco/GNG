#ifndef GRAPH_NULL_GNG_ALGO_H
#define GRAPH_NULL_GNG_ALGO_H

#include "../graph.h"
#include <cmath>

template <class Trait>
class GNGAlgorithm {
protected:
    typedef typename Graph<Trait>::NodeContent NodeContent;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    typedef ::Node<Graph<Trait>>* NodePtr;

    double mean_error_sum = 0; //Sum of mean error until reset
    int mean_error_check = 500; // Mean error iteration check

    int SIGMA; //Error global maximo
    double ALFA; // Decremento del error en la insercion para u, v, r
    double BETA; //Decremento del error para cada iteracion
    double E_W; // Movimiento de los nodos mas cercanos hacia el input
    double E_N; //Movimiento de los vecinos del nodo mas cercano hacia el input
    int MAX_AGE; //Maxima vida de iteraciones por nodo
    int MAX_NODES; //Maximos nodos


    unsigned int SCREEN_HEIGHT;
    unsigned int SCREEN_WIDTH;

    unsigned int iteracion = 1;

    Graph<Trait> base_graph; 
   

    //Finds the neighbor with the max error of said node
    auto findMaxErrorLink(NodePtr node) const;

    //Log the mean error to console
    void logMeanError(double sum);

public:
    GNGAlgorithm(unsigned int screen_width, unsigned int screen_height) : SCREEN_HEIGHT(screen_height), SCREEN_WIDTH(screen_width) {
        this->SIGMA = 300; 
        this->ALFA = 0.5;
        this->BETA =0.0005; 
        this->E_W = 0.05; 
        this->E_N = 0.0006; 
        this->MAX_AGE = 100; 
        this->MAX_NODES = 70;
    }
    bool isConnected(NodePtr node1, NodePtr node2) const;
    double getDistance(NodePtr node, pair<int, int> input) const;
    double getDistancePoint(pair<int,int> point, pair<int, int> input) const;
    int getMaxAge() const; 
    
    //Finds the node with the max error
    auto findMaxError(Graph<Trait> &graph) const;
    double findMaxMeanError() const;
    unsigned int get_iteracion() const{
        return this->iteracion;
    }
    auto get_graph() const{
        return base_graph;
    }
    virtual void exec(pair<int, int> input); 
    virtual void init();

};

template <class Trait>
class UGNGAlgorithm : public GNGAlgorithm<Trait> { 
    typedef typename Graph<Trait>::NodeContent NodeContent;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    typedef ::Node<Graph<Trait>>* NodePtr;


    double K; //Medida de utilidad para eliminar un nodo

    //Encuentra el nodo con la minima utilidad
	auto findMinUtility(Graph<Trait> &graph) const;

    //Encuentra el vecino con menor utilidad
    auto findMinUtilityNear(NodePtr node_ptr) const;

 public:
    UGNGAlgorithm(unsigned int screen_width, unsigned int screen_height) : GNGAlgorithm<Trait>(screen_height, screen_width){ 
        this->K = 1;
        this->SIGMA = 600; 
        this->ALFA = 0.5;  
        this->BETA =0.0005; 
        this->E_W = 0.09; 
        this->E_N = 0.006; 
        this->MAX_AGE = 100; 
        this->MAX_NODES = 60;
    }
    virtual void init();
    virtual	void exec(pair<int, int> input);
};
#include "gng_algo.cpp"
#endif //GRAPH_NULL_GNG_H

