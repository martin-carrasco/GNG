//
// Created by martin on 10/23/18.
//

#ifndef GRAPH_NULL_GNG_H
#define GRAPH_NULL_GNG_H

#include "../graph.h"
#include "SFML/Graphics.hpp"

using namespace std;

template <class GNGTrait>
class InputGenerator;

template <class GNGTrait>
class GNGAlgorithm{
    typedef typename Graph<GNGTrait>::NodeContent NodeContent;
    typedef typename Graph<GNGTrait>::Edge* EdgePtr;
    typedef typename Graph<GNGTrait>::Node* NodePtr;

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
    Node<Graph<GNGTrait>>* find_max_error(Graph<GNGTrait> &graph);
    //Finds the neighbor of node with the highest error value
    Node<Graph<GNGTrait>>* find_max_error_connection(NodePtr node);

public:
    void init();
    bool hay_coneccion(NodePtr node1, NodePtr node2);
    double calcular_distancia(NodePtr node, sf::Vertex input);
    void exec(sf::Vertex input);
    int get_iteracion(){
        return iteracion;
    }
    Graph<GNGTrait> get_graph(){
        return base_graph;
    }

};

template <class GNGTrait>
class InputGenerator{
    typedef typename Graph<GNGTrait>::Node Node;
    typedef Node* NodePtr;
    //Vectores de input que conforman la imagen
    vector<sf::Vertex> pos_vector;

public:
    InputGenerator(sf::VertexArray vertexArray);
    sf::Vertex pop();
    unsigned long size();
};
template<class GNGTrait>
class GNGContainer{
    typedef typename Graph<GNGTrait>::Node Node;
    typedef typename Graph<GNGTrait>::Edge Edge;
    typedef Node* NodePtr;
    typedef Edge* EdgePtr;


    InputGenerator<GNGTrait> inpt_gen;
    GNGAlgorithm<GNGTrait> algo;
    //Ventana del GNG
    sf::RenderWindow window;

    //Vector de nodos dibujadas en cada ciclo
    std::vector<sf::CircleShape> nodes;

    //Vector de aristas dibujadas en cada ciclo
    std::vector<sf::Vertex*> edges;

    //Vector de posicion de lineas auxiliar
    std::vector<sf::Vector2f> lineas;

    //Auxiliar para dibujo
    bool is_drawing = false;

    //Variable de pause
    bool is_running = true;
    void draw_node(NodePtr node);
    void draw_edge(NodePtr start_node, NodePtr end_node);

public:
    GNGContainer(){};
    void init();
    void start();
};

#include "gng.cpp"
#endif //GRAPH_NULL_GNG_H