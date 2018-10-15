#ifndef GRAPH_NULL_PRIM_H
#define GRAPH_NULL_PRIM_H
#include "../graph.h"

template <class CustomTrait>
class Prim {
private:
    typedef ::Node<Graph<CustomTrait>>* NodePtr;
    typedef ::Edge<Graph<CustomTrait>>* EdgePtr;
    typedef typename Graph<CustomTrait>::EdgeContent EdgeContent;
    typedef typename Graph<CustomTrait>::NodeContent NodeContent;
    std::priority_queue<Node<Graph<CustomTrait>>*> nodes;
    Graph<CustomTrait> base_graph;
public:
    Prim(Graph<CustomTrait> graph);
    auto generate_minimum_spanning_tree();
    void describe();
};

#include "prim.cpp"
#endif //GRAPH_NULL_PRIM_H
