#include <iostream>
#include <stdexcept>
using namespace std;

template <class CustomTrait>
auto Graph<CustomTrait>::getEdgesVector() {
    vector<Edge*> visited;
    for(Node* node : nodes_vector){
        for(Edge* e : node->getEdges())
            if(find(visited.begin() ,visited.end() ,e) == visited.end())
                visited.push_back(e);
    }
    return visited;
}

template <class CustomTrait>
unsigned long Graph<CustomTrait>::size(){
    nodes_vector.size();
}

template <class CustomTrait>
auto Graph<CustomTrait>::findEdge(Node* start, Node* end, EdgeContent content){
    for(Edge* e : start->getEdges()){
	    if(e->getDest(start) == end && e->getContent() == content)
		    return  e;
    }
    return (Edge*) nullptr;
}
template <class CustomTrait>
void Graph<CustomTrait>::clear(){
    for(Node* n : nodes_vector){
        deleteNode(n->getContent());
    }
}
template <class CustomTrait>
auto Graph<CustomTrait>::findNode(NodeContent content){
    for(auto it = nodes_vector.begin(); it != nodes_vector.end();it++){
        if((*it)->getContent() == content)
            return *it;
    }

    return (Node*)nullptr;
}

template <class CustomTrait>
bool Graph<CustomTrait>::insertNode(NodeContent node_content){
    Node *node = new Node();
    node->setContent(node_content);
    nodes_vector.push_back(node);
    return true;
}
template <class CustomTrait>
void Graph<CustomTrait>::insertEdge(EdgeContent edge_content, NodeContent start, NodeContent end, bool is_directed) {
    if (start == end)
	    throw invalid_argument("Start edge same as End edge");

    Edge *edge = new Edge();
    Node * a = findNode(start);
    Node * b = findNode(end);

    if (a == nullptr || b == nullptr)
        throw invalid_argument("Some node didnt exist");

    edge->setVertices(a, b);
    edge->setContent(edge_content);
    edge->setDirected(is_directed);

    a->addEdge(edge);
    b->addEdge(edge);
}

template<class CustomTrait>
void Graph<CustomTrait>::deleteNode(NodeContent content){
    Node* node = findNode(content);

    if(node == nullptr)
        throw invalid_argument("Node did not exist");


    EdgesList tmpEdgeList = node->getEdges();

    //Iteramos alrededor de todas las aristas del nodo y procedemos a eliminarlas
    for(auto it = tmpEdgeList.begin();it != tmpEdgeList.end();it++){
	deleteEdge(content, (*it)->getDest(node)->getContent(), (*it)->getContent());
    }

    //Finalmente hacemos una iteracion en la el vector de nodos, lo borramos de ahi
    //y liberamos el puntero
    for(nodes_iterator = nodes_vector.begin();nodes_iterator != nodes_vector.end();nodes_iterator++){
        if(*nodes_iterator == node)
            nodes_vector.erase(nodes_iterator);
    }

    delete node;
}
template <class CustomTrait>
void Graph<CustomTrait>::deleteEdge(NodeContent start, NodeContent end, EdgeContent content) {
    Node* start_node = findNode(start);
    Node* end_node = findNode(end);

    if(start_node == nullptr || end_node == nullptr)
        throw invalid_argument("One node content did not exist");

    Edge* edge = findEdge(start_node, end_node, content);

    if(edge == nullptr)
        throw invalid_argument("Edge between the nodes did not exist");


    start_node->removeEdge(edge);
    end_node->removeEdge(edge);

    //delete edge;
}

template <class CustomTrait>
bool Graph<CustomTrait>::isDirected() {
    for(nodes_iterator = nodes_vector.begin();nodes_iterator != nodes_vector.end();nodes_iterator++){
        Node* current = *nodes_iterator;
        for(Edge* e : current->getEdges()){
            if(e->isDirected())
                return true;
        }
    }
    return false;
}


template <class CustomTrait>
void Graph<CustomTrait>::describe() {
    for(auto node : nodes_vector){
        std::cout << "Node " << node->getContent() << " -> ";
        for(auto e : node->getEdges()){
            Node* dest = e->getDest(node);
            if(dest == nullptr)
                continue;
            cout << "Node " << dest->getContent() << " ";
        }
        std::cout << std::endl;
    }
}
template <class CustomTrait>
void Graph<CustomTrait>::describeHelper(NodesVector to_describe){
    for(auto node : to_describe){
        std::cout << "Node " << node->getContent() << " -> ";
        for(auto e : node->getEdges()){
            Node* dest = e->getDest(node);
            if(dest == nullptr)
                continue;
            cout << "Node " << dest->getContent() << " ";
        }
        std::cout << std::endl;
    }
}

template <class CustomTrait>
auto Graph<CustomTrait>::getNodesVector(){
    return nodes_vector;
}
