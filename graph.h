#ifndef GRAPH_NULL_GRAPH_H
#define GRAPH_NULL_GRAPH_H
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>

template <class GraphTrait>
class Node {
private:
    typedef typename GraphTrait::NodeContent    NodeContent;
    typedef typename GraphTrait::EdgeContent    EdgeContent;
    typedef typename GraphTrait::Edge           Edge;
    typedef typename GraphTrait::EdgesList      EdgesList;

    EdgesList edges_list;
    NodeContent node_content;

public:
    Node () = default;

    EdgesList getEdges_list() const {
        return edges_list;
    }

    void setEdges_list(EdgesList edges_list) {
        Node::edges_list = edges_list;
    }

    void addEdge(Edge * edge) {
        this->edges_list.push_back(edge);
    }

    NodeContent getNode_content() const {
        return node_content;
    }

    void setNode_content(NodeContent node_content) {
        Node::node_content = node_content;
    }
};

template <class GraphTrait>
class Edge {
private:
    typedef typename GraphTrait::EdgeContent EdgeContent;
    typedef typename GraphTrait::Node Node;

    bool is_directed;
    EdgeContent edge_content;
    Node * vertices[2];

public:
    Edge () = default;

    bool isIs_directed() const {
        return is_directed;
    }

    void setIs_directed(bool is_directed) {
        Edge::is_directed = is_directed;
    }

    EdgeContent getEdge_content() const {
        return edge_content;
    }

    void setEdge_content(EdgeContent edge_content) {
        Edge::edge_content = edge_content;
    }

    Node* getDest(Node* node){
        if(is_directed){
            if(vertices[1] == node)
                return nullptr;

            return vertices[1];
        }

        return node == vertices[0] ? vertices[1] : vertices[0];
    }

    Node *const *getVertices() const {
        return vertices;
    }

    void setVertices(Node * a, Node * b) {
        this->vertices[0] = a;
        this->vertices[1] = b;
    }
};

template <class Trait>
class Graph {
public:
    typedef ::Node< Graph<Trait> > Node;
    typedef ::Edge< Graph<Trait> > Edge;

    typedef std::vector<Node*>  NodesVector;
    typedef std::queue<Node*>   NodesQueue;
    typedef std::stack<Node*>   NodesStack;
    typedef std::vector<Edge*>    EdgesList;

    typedef typename Trait::NodeContent     NodeContent;
    typedef typename Trait::EdgeContent     EdgeContent;
    typedef typename NodesVector::iterator  NodesIterator;
    typedef typename EdgesList::iterator    EdgesIterator;
private:
    NodesVector nodes_vector;

    NodesIterator nodes_iterator;
    EdgesIterator edges_iterator;

    auto find_node(NodeContent content);
    auto find_edge(Node* start, Node* end, EdgeContent content);

public:
    auto get_edgesVector();
    auto get_nodesVector();

    bool is_directed();
    unsigned long size();
    unsigned long num_edges();

    bool insert_node(NodeContent node_content);
    void insert_edge(EdgeContent edge_content, NodeContent start, NodeContent end, bool is_directed);

    bool delete_node(NodeContent content);
    bool delete_edge(NodeContent start, NodeContent end, EdgeContent content);

    auto bfs(NodeContent content);
    auto dfs(NodeContent content);

    // NodesVector dijkstra(NodeContent start);

    void describe();
    void describe_helper(NodesVector to_describe);
};

class CustomTrait {
public:
    typedef std::string NodeContent;
    typedef int EdgeContent;
};
#include "graph.cpp"

#endif //GRAPH_NULL_GRAPH_H

