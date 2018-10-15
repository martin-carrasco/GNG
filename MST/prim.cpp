#define MAX 1000
#include <map>

template <class CustomTrait>
Prim<CustomTrait>::Prim(Graph<CustomTrait> graph) {
    if(graph.is_directed())
        throw "Cannot make a directed Graph a prim subgraph";
    this->base_graph = graph;
}
template <class CustomTrait>
auto Prim<CustomTrait>::generate_minimum_spanning_tree() {
    if (base_graph.size() == 0)
        return *this;

    Graph <CustomTrait> min_span_tree;

    //TODO Same as first if ????
    NodePtr start_node = base_graph.get_nodesVector()[0];
    if (start_node == nullptr)
        return *this;

    priority_queue<NodePtr> queue;
    vector<NodePtr> visited;
    std::map<NodePtr, EdgeContent> keys;

    //Inicializa el vector de distancias y el MST vacio
    for(NodePtr n : base_graph.get_nodesVector()){
        keys.insert(make_pair(n, MAX));
        min_span_tree.insert_node(n->getNode_content());
    }

    queue.push(start_node);
    while(!queue.empty()){
        NodePtr node = queue.top();
        queue.pop();

        //Nodo marca como visitado
        visited.push_back(node);

        for(EdgePtr ep : node->getEdges_list()){
            NodePtr dest_node = ep->getDest(node);
            EdgeContent weight = ep->getEdge_content();

            //TODO crear un MAX con template
            if(find(visited.begin(), visited.end(), dest_node) == visited.end() && keys[dest_node] > weight){
                keys[dest_node] = weight;
                queue.push(dest_node);
                min_span_tree.insert_edge(weight, node->getNode_content(), dest_node->getNode_content(), false);
            }
        }
    }
    return Prim<CustomTrait>(min_span_tree);
}
template <class CustomTrait>
void Prim<CustomTrait>::describe() {
    base_graph.describe();
}