#include "graph.h"
#include <algorithm>
#include <stack>
#include <queue>


Node* find_node(NodeContent content){
	for(NodesIterator it = NodesVector.begin();it != NodesVector.end();it++){
		if((*it)->node_content == content)
			return *it;
	}
	return nullptr;
}

void Graph::insert_node(NodeContent node_content){
	Node* node = new Node();
	node.node_content = node_content;
	NodesVector.push_back(node);
}

void Graph::delete_node(NodeContent node_content){
	for(NodesIterator it = NodesVector.begin();it != NodesVector.end();it++){
		if((*it)->node_content == node_content){
			NodesVector.erase(it);
		}
	}
}

void Graph::insert_edge(EdgeContent edge_content, NodeContent start, NodeContentint end, bool is_directed){
	Edge* edge = new Edge();
	Node * vertices = new Node[2];
	vertices[0] = find_node(start);
	vertices[1] = find_node(end);

	if(vertices[1] == nullptr || vertices[0] == nullptr)
		return;

	edge->edge_content = edge_content;
	edge->is_directed = is_directed;
	edge->vertices[0] = vertices[0];
	edges->vertices[1] = vertices[1];


	vertices[1]->edge_list.push_back(edge);
	vertices[0]->edge_list.push_back(edge);
}

void Graph::delete_edge(NodeContent start, NodeContent end){
	
	vector<Edge*>::iterator rem_dest = dest_node->edges.end();
	vector<Edge*>::iterator rem_source = source_node->edges.end();

	for(vector<Edge*>::iterator it = source_node->edges.begin(); it != source_node->edges.end();it++){
		Edge* e = (*it);
		if(e == edge){
			rem_source = it;
			break;
		}
	}

	for(vector<Edge*>::iterator it = dest_node->edges.begin(); it != dest_node->edges.end();it++){
		Edge* e = (*it);
		if(e == edge){
			rem_dest = it;
			break;
		}
	}
	if(rem_dest != dest_node->edges.end())
		dest_node->edges.erase(rem_dest);
	if(rem_source != source_node->edges.end())
		source_node->edges.erase(rem_source);
}

//Depth-first Search
NodesVector Graph::dfs(NodeContent content){
	NodesVector discovered;
	NodesStack node_stack;
	Node* node = find_node(content);
	
	if(content == nullptr)
		return discovered;
		
	node_stack.push(node);
	while(!node_stack.empty()){
		Node* current = node_stack.top();
		node_stack.pop();
		if(find(discovered.begin(), discovered.end(), current)  == discovered.end()){
			discovered.push_back(current);
			for(auto edge : current->edges){
				node_stack.push(edge->dest);
			}
		}
		
	}
	return discovered;
}

//Bredth-first Search
NodesVector Graph::bfs(NodeContent content){
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

vector<Edge*> Graph::kruskal(){
	vector<Edge*> aristas;
	for(auto node : graph_strct){
		aristas.insert(aristas.end(), node->edges.begin(), node->edges.end());
	}
}

void Graph::describe(){
	for(auto node : nodes_vector){
		cout << "Node" << node->node_content << " -> ";
		for(auto e : node->edge_list){
			cout << "Node" << e->vertices[1]->node_content; 
		}
		cout << endl;
	}
}