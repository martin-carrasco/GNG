#include <iostream>
using namespace std;

template <class CustomTrait>
auto Graph<CustomTrait>::get_edgesVector() {
    vector<Edge*> visited;
    for(Node* node : nodes_vector){
        for(Edge* e : node->getEdges_list())
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
auto Graph<CustomTrait>::find_edge(Node* start, Node* end, EdgeContent content){
    for(Edge* e : start->getEdges_list()){
        if(e->getEdge_content() == content &&
           e->getVertices()[0] == start &&
           e->getVertices()[1] == end)

            return e;
    }
    return (Edge*) nullptr;
}
template <class CustomTrait>
void Graph<CustomTrait>::clear(){
    for(Node* n : nodes_vector){
        delete_node(n->getNode_content());
    }
}
template <class CustomTrait>
auto Graph<CustomTrait>::find_node(NodeContent content){
    for(auto it = nodes_vector.begin(); it != nodes_vector.end();it++){
        if((*it)->getNode_content() == content)
            return *it;
    }

    return (Node*)nullptr;
}

template <class CustomTrait>
bool Graph<CustomTrait>::insert_node(NodeContent node_content){
    Node *node = new Node();
    node->setNode_content(node_content);
    nodes_vector.push_back(node);
    return true;
}
template <class CustomTrait>
void Graph<CustomTrait>::insert_edge(EdgeContent edge_content, NodeContent start, NodeContent end, bool is_directed) {
    if (start == end) {
        return;
    }

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

template<class CustomTrait>
bool Graph<CustomTrait>::delete_node(NodeContent content){
    Node* node = find_node(content);

    if(node == nullptr)
        return false;


    EdgesList tmpEdgeList = node->getEdges_list();

    //Iteramos en la lista de aristas que estan relacionadas al nodo que buscamos borrar
    //LLamamos a la funcion delete_edge con el nodo relacionado y aseguramos que se borre adecuadamente
    for(auto it = tmpEdgeList.begin();it != tmpEdgeList.end();it++){

        Edge* tmp_edge = *it;
        if(tmp_edge->getVertices()[0] == node || tmp_edge->getVertices()[1] == node) {
            NodeContent content_end = tmp_edge->getVertices()[1] == node
                                      ? tmp_edge->getVertices()[0]->getNode_content()
                                      : content; //Determina el nodo opuesto

            delete_edge(content_end, content, tmp_edge->getEdge_content());
        }

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
bool Graph<CustomTrait>::delete_edge(NodeContent start, NodeContent end, EdgeContent content) {
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

template <class CustomTrait>
bool Graph<CustomTrait>::is_directed() {
    for(nodes_iterator = nodes_vector.begin();nodes_iterator != nodes_vector.end();nodes_iterator++){
        Node* current = *nodes_iterator;
        for(Edge* e : current->getEdges_list()){
            if(e->isIs_directed())
                return true;
        }
    }
    return false;
}


template <class CustomTrait>
void Graph<CustomTrait>::describe() {
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
template <class CustomTrait>
void Graph<CustomTrait>::describe_helper(NodesVector to_describe){
    for(auto node : to_describe){
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

template <class CustomTrait>
auto Graph<CustomTrait>::get_nodesVector(){
    return nodes_vector;
}
