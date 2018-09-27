#include <list>
#include <vector>
using namespace std;

struct Node;
template <class GraphTrait>
class Edge{
private:
	typedef typename GraphTrait::EdgeContent EdgeContent;
    typedef typename GraphTrait::Node Node;
public:
	bool is_directed;
	EdgeContent edge_content;
	Node* vertices[2];

	bool operator==(Edge &e){
		return weight==e.weight && 
			dest == (e.dest) && 
			source== (e.source);
	}
};

template <class GraphTrait>
class Node{
private:
	typedef typename GraphTrait::NodeContent    NodeContent;
    typedef typename GraphTrait::EdgeContent    EdgeContent;
    typedef typename GraphTrait::Edge           Edge;
    typedef typename GraphTrait::EdgesList      EdgesList;
public:
	Node() = default;
    EdgesList edges_list;
    NodeContent node_content;
	bool operator==(Node &n){
		return edges==n.edges;
	}
};

template <class Trait>
class Graph{
private:
	NodesVector nodes_vector;
    NodesIterator nodes_iterator;
    EdgesIterator edges_iterator;
    Node* head = nullptr;
public:
	typedef Node< Graph<Trait> > Node;
    typedef Edge< Graph<Trait> > Edge;
    
    typedef std::vector<Node*>  NodesVector;
    typedef std::list<Edge*>    EdgesList;
    
    typedef typename Trait::NodeContent     NodeContent;
    typedef typename Trait::EdgeContent     EdgeContent;
    typedef typename NodesVector::iterator  NodesIterator;
    typedef typename EdgesList::iterator    EdgesIterator;

	Graph() = default;                              

	void find(NodeContent content, Node **& ptr);
	void insert_node(NodeContent node_content); // Crea un node y lo añade con el contenido dado
	void delete_node(NodeContent node); //Remueve un node basado en su contenido
	void insert_edge(EdgeContent edge, NodeContent start, NodeContent end, bool is_directed); //Añade una arista
	void delete_edge(NodeContent start, NodeContent end); //Remueve una arista
	void describe();
	vector<Node*> dfs(NodeContent node);
	vector<Node*> bfs(NodeContent node);
	vector<Edge*> kruskal();
		
		/*
		int show_weight(int source_vertex, int dest_vertex);
		vector<int> shortest_path(int source_vertex, int dest_vertex); //Camino minimo de Dijstra
		Node bfs(int source); //Bredth.-first-search
		int next(int current, int target) // Devuelve el siguiente nodo a visitar
		*/
};
class Iterator
struct EdgeBase{
	int weight;
	string label;
};

class CustomTrait{
	typedef string NodeContent;
	typedef int EdgeContent;
};