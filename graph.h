#ifndef GRAPH_NULL_GRAPH_H
#define GRAPH_NULL_GRAPH_H
#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <assert.h>
#include <unordered_map>
#include <fstream>
#include <map>
#include <stdexcept>

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

    EdgesList getEdges() const {
        return edges_list;
    }

    void setEdges(EdgesList edges_list) {
        Node::edges_list = edges_list;
    }

    void addEdge(Edge * edge) {
        this->edges_list.push_back(edge);
    }
    void removeEdge(Edge* edge){
	auto it = find(edges_list.begin(), edges_list.end(), edge);
	if(it != edges_list.end()){	
		edges_list.erase(it);
		return;
	}
	throw std::invalid_argument("Edge not found");
    }

    NodeContent getContent() const {
        return node_content;
    }

    void setContent(NodeContent node_content) {
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

    bool isDirected() const {
        return is_directed;
    }

    void setDirected(bool is_directed) {
        Edge::is_directed = is_directed;
    }

    EdgeContent getContent() const {
        return edge_content;
    }

    void setContent(EdgeContent edge_content) {
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

    typedef std::vector<Node*> NodesVector;
    typedef std::queue<Node*> NodesQueue;
    typedef std::stack<Node*> NodesStack;
    typedef std::vector<Edge*> EdgesList;

    typedef typename Trait::NodeContent     NodeContent;
    typedef typename Trait::EdgeContent     EdgeContent;
    typedef typename NodesVector::iterator  NodesIterator;
    typedef typename EdgesList::iterator    EdgesIterator;
private:
    NodesVector nodes_vector;

    NodesIterator nodes_iterator;
    EdgesIterator edges_iterator;

    auto findNode(NodeContent content);
    auto findEdge(Node* start, Node* end, EdgeContent content);

public:
    auto getConnection(Node* node1, Node* node2);
    auto getEdgesVector();
    auto getNodesVector();

    bool isDirected();
    unsigned long size();
    unsigned long num_edges();

    bool insertNode(NodeContent node_content);
    void insertEdge(EdgeContent edge_content, NodeContent start, NodeContent end, bool is_directed);

    void deleteNode(NodeContent content);
    void deleteEdge(NodeContent start, NodeContent end, EdgeContent content);

    void clear();

    void describe();
    void describeHelper(NodesVector to_describe);
};

class GNGTrait{
public:
    struct Node{
        double pos[2];
        double error;
        bool operator==(Node &content){
            return pos[0] == content.pos[0] && pos[1] == content.pos[1] && error == content.error;
        }
    };
    typedef Node NodeContent;
    typedef int EdgeContent;
};
class UGNGTrait{
public:
	struct Node{
		double pos[2];
		double error;
		double U;
		bool operator==(Node &content){
		    return pos[0] == content.pos[0] && pos[1] == content.pos[1] && error == content.error;
		}
	};
	typedef Node NodeContent;
	typedef int EdgeContent;
};
class CustomTrait {
public:
    typedef std::string NodeContent;
    typedef int EdgeContent;
};
#include "graph.cpp"

#endif //GRAPH_NULL_GRAPH_H
