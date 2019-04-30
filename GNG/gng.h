//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "../graph.h"
#include "SFML/Graphics.hpp"
#include <random>
#include <functional>


using namespace std;

template <class GNGTrait>
class InputGenerator;

template <class GNGTrait>
class GNGAlgorithm{
private:
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

    unsigned int iteracion = 1;
    Graph<GNGTrait> base_graph;

    //Encuentra el nodo con la minima utilidad
    auto findMinUtility(Graph<GNGTrait> &graph);

    //Find the node with the highest error in the graph
    auto findMaxError(Graph<GNGTrait> &graph);
    
    //Finds the neighbor of node with the highest error value
    auto findMaxErrorLink(NodePtr node);

public:
    GNGAlgorithm() = default;
    void init();
    bool isConnected(NodePtr node1, NodePtr node2);
    double getDistance(NodePtr node, sf::Vertex input);
    void exec(sf::Vertex input);
    int get_iteracion(){
        return iteracion;
    }
    auto get_graph(){
        return base_graph;
    }

};

template <class GNGTrait>
class InputGenerator{
protected:
    typedef ::Node<Graph<GNGTrait>>* NodePtr;
    //Vectores de input que conforman la imagen
    vector<sf::Vertex> pos_vector;
public:
    InputGenerator(vector<sf::VertexArray> vec);
    virtual sf::Vertex pop() = 0;
    unsigned long size();

};

template <class GNGTrait>
class UniformDistributionInputGenerator : public InputGenerator<GNGTrait>{
private:
	default_random_engine re;
	uniform_int_distribution<int> dist;
public:
	UniformDistributionInputGenerator(vector<sf::VertexArray> vec) : InputGenerator<GNGTrait>(vec) {}
	sf::Vertex pop();
};
template <class GNGTrait>
class DefaultInputGenerator : public InputGenerator<GNGTrait>{
public:
	DefaultInputGenerator(vector<sf::VertexArray> vec) : InputGenerator<GNGTrait>(vec) {}
	sf::Vertex pop();
};

template<class GNGTrait>
class GNGContainer{
    typedef ::Node<Graph<GNGTrait>>* NodePtr;
    typedef ::Edge<Graph<GNGTrait>>* EdgePtr;
    GNGAlgorithm<GNGTrait> algo;
    //Ventana del GNG
    sf::RenderWindow window;

    //Vector de posicion de lineas auxiliar
    std::vector<sf::Vector2f> lineas;

    //Auxiliar para dibujo
    bool is_drawing = false;

    //Variable de pause
    bool is_running = false;
public:
    GNGContainer() = default;
    void init();
    void start();
    void start_moving();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H
