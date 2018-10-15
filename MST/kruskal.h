#ifndef GRAPH_NULL_KRUSKAL_H
#define GRAPH_NULL_KRUSKAL_H

#include "../graph.h"

template<class CustomTrait>
class Kruskal {
public:
    friend class Graph<CustomTrait>;
    typedef ::Node<Graph<CustomTrait>>* NodePtr;
    typedef ::Edge<Graph<CustomTrait>>* EdgePtr;

private:
    Graph<CustomTrait> graph;

public:
    Kruskal(Graph<CustomTrait> graph);

    auto generate_minimum_spanning_tree();
};
#include "kruskal.cpp"
#endif //GRAPH_NULL_KRUSKAL_H
