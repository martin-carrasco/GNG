#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <assert.h>

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
	typedef std::vector<Edge*>    EdgesList;

	typedef typename Trait::NodeContent     NodeContent;
	typedef typename Trait::EdgeContent     EdgeContent;
	typedef typename NodesVector::iterator  NodesIterator;
	typedef typename EdgesList::iterator    EdgesIterator;

private:
	NodesVector nodes_vector;

	NodesIterator nodes_iterator;
	EdgesIterator edges_iterator;

	Node* find_node(NodeContent content){
		for(auto it = nodes_vector.begin(); it != nodes_vector.end();it++){
			if((*it)->getNode_content() == content)
				return *it;
		}

		return nullptr;
	}
	Edge* find_edge(Node* start, Node* end, EdgeContent content){
		for(Edge* e : start->getEdges_list()){
			if(e->getEdge_content() == content &&
				e->getVertices()[0] == start &&
				e->getVertices()[1] == end)

				return e;
		}
		return nullptr;
	}
public:
	Graph() = default;

	bool insert_node(NodeContent node_content) {
		Node *node = new Node();
		node->setNode_content(node_content);
		nodes_vector.push_back(node);
		return true;
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

	bool delete_node(NodeContent content){
		Node* node = find_node(content);

		if(node == nullptr)
			return false;

		//Iteramos en la lista de aristas que estan relacionadas al nodo que buscamos borrar
		//LLamamos a la funcion delete_edge con el nodo relacionado y aseguramos que se borre adecuadamente
		for(auto it = node->getEdges_list().begin();
				it != node->getEdges_list().end();
				it++){

			if(*it == nullptr) //  ???
				continue;

			Edge* tmp_edge = *it;
			if(tmp_edge->getVertices()[0] == node || tmp_edge->getVertices()[1] == node) {
				NodeContent content_end = tmp_edge->getVertices()[0] == node
						? tmp_edge->getVertices()[1]->getNode_content()
						: content;
				delete_edge(content, content_end, tmp_edge->getEdge_content());
				break;
			}

		}

		//Finalmente hacemos una iteracion en la el vector de nodos, lo borramos de ahi
		//y liberamos el puntero
		for(nodes_iterator = nodes_vector.begin();nodes_iterator != nodes_vector.end();nodes_iterator++){
			if(*nodes_iterator == node)
				nodes_vector.erase(nodes_iterator);break;
		}

		delete node;
	}

	bool delete_edge(NodeContent start, NodeContent end, EdgeContent content) {
		Node* start_node = find_node(start);
		Node* end_node = find_node(end);

		if(start_node == nullptr || end_node == nullptr)
			return false;

		Edge* edge = find_edge(start_node, end_node, content);

		if(edge == nullptr)
			return false;


		EdgesList tmpEdgeListStart = start_node->getEdges_list();
		EdgesList tmpEdgeListEnd = end_node->getEdges_list();
		Edge* to_be_delted = nullptr;

		//Iteramos a lo largo de las dos listas de edges y al encontrarlo
		//lo añadimos a un vector temporal que despues reemplazaremos por el vector
		//contenido en cada Node
		//Finalmente guardamos el puntero a Edge para poder liberar su memoria

		for(edges_iterator = tmpEdgeListStart.begin();
					edges_iterator != tmpEdgeListStart.end();edges_iterator++){
			Edge* tmp_edge = *edges_iterator;
			if(tmp_edge == edge) {
				to_be_delted = tmp_edge;
				tmpEdgeListStart.erase(edges_iterator);
				break;
			}
		}

		for(edges_iterator = tmpEdgeListEnd.begin();
			edges_iterator != tmpEdgeListEnd.end();edges_iterator++){
			Edge* tmp_edge = *edges_iterator;
			if(tmp_edge == edge) {
				assert(tmp_edge == to_be_delted);
				tmpEdgeListEnd.erase(edges_iterator);
				break;
			}
		}

		start_node->setEdges_list(tmpEdgeListStart);
		end_node->setEdges_list(tmpEdgeListEnd);

		delete to_be_delted;

		return true;
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


	/*NodesVector prim(NodeContent content){
		Node* base_node = find_node(content);

		if(base_node== nullptr)
			return ;


	}*/

	void describe() {
		for(auto node : nodes_vector){
			std::cout << "Node " << node->getNode_content() << " -> ";
			for(auto e : node->getEdges_list()){
				Node* dest = e->getDest(node);
				if(dest == nullptr)
					continue;
				cout << "Node " << dest->getNode_content() << " ";
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

	graph.insert_edge(10, "A", "B", true);
	graph.insert_edge(10, "C", "B", true);
	graph.insert_edge(5, "C", "A", true);

	graph.delete_node("B");

	graph.describe();

	return 0;
}
