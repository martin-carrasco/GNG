#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>

using namespace std;

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
	typedef std::list<Edge*>    EdgesList;

	typedef typename Trait::NodeContent     NodeContent;
	typedef typename Trait::EdgeContent     EdgeContent;
	typedef typename NodesVector::iterator  NodesIterator;
	typedef typename EdgesList::iterator    EdgesIterator;

private:
	NodesVector nodes_vector;

	NodesIterator nodes_iterator;
	EdgesIterator edges_iterator;

public:
	Graph() = default;

	Node* find_node(NodeContent content){
		for(NodesIterator it = nodes_vector.begin(); it != nodes_vector.end();it++){
			if((*it)->getNode_content() == content)
				return *it;
		}

		return nullptr;
	}

	bool insert_node(NodeContent node_content) {
		Node *node = new Node();
		node->setNode_content(node_content);
		nodes_vector.push_back(node);
		return true;
	}

	bool delete_node(NodeContent node_content) {
		/* ... */
		return false;
	}

	void insert_edge(EdgeContent edge_content, NodeContent start, NodeContent end, bool is_directed) {
		Edge *edge = new Edge();
		Node * a = find_node(start);
		Node * b = find_node(end);

		if (a == nullptr || b == nullptr)
			return;

		edge->setVertices(a, b);
		edge->setEdge_content(edge_content);
		edge->setIs_directed(is_directed);

		a->addEdge(edge);
		b->addEdge(edge);
	}

	bool delete_edge(NodeContent start, NodeContent end) {
		/* ... */
		return false;
	}

	NodesVector bfs (NodeContent content){
		NodesVector discovered;
		NodesQueue node_queue;
		Node* node = find_node(content);

		if(content == nullptr)
			return discovered;


		node_queue.push(node);
		while(!node_queue.empty()){
			Node* current = node_queue.next();
			node_queue.pop();
			if(find(discovered.begin(), discovered.end(), current)  == discovered.end()){
				discovered.push_back(current);
				for(auto edge : current->edges){
					node_queue.push(edge->dest);
				}
			}

		}

		return discovered;
	}

	void describe() {
		for(auto node : nodes_vector){
			std::cout << "Node " << node->getNode_content() << " -> ";
			for(auto e : node->getEdges_list()){
				cout << "Node " << (e->getDest(node))->getNode_content() << " ";
			}
			std::cout << std::endl;
		}
	}
};

class CustomTrait {
public:
	typedef std::string NodeContent;
	typedef int EdgeContent;
};

int main(int argc, char *argv[]) {
	Graph<CustomTrait> graph;

	graph.insert_node("A");
	graph.insert_node("B");
	graph.insert_node("C");

	graph.insert_edge(10, "A", "B", false);
	graph.insert_edge(10, "A", "C", true);

	graph.describe();

	return 0;
}
