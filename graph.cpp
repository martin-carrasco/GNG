#include "graph.h"
#include <algorithm>
#include <stack>
#include <queue>



void Graph:find(NodeContent node_content, Node **& ptr){
	while(ptr != nullptr && (*ptr)->node_content != node_content){
		ptr = &((*ptr)->)
	}
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

void Graph::insert_edge(EdgeContent edge, NodeContent start, NodeContentint end, bool is_directed){

	ource_node->edges.push_back(e);
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
vector<Node*> Graph::dfs(NodeContent node){
	vector<Node*> discovered;
	stack<Node*> node_stack;
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
vector<Node*> Graph::bfs(NodeContent node){
	vector<Node*> discovered;
	queue<Node*> node_queue;
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
}

vector<Edge*> Graph::kruskal(){
	vector<Edge*> aristas;
	for(auto node : graph_strct){
		aristas.insert(aristas.end(), node->edges.begin(), node->edges.end());
	}
}

void Graph::describe(){
	for(auto node : graph_strct){
		cout << "Node" << node->tag << " -> ";
		for(auto e : node->edges){
			cout << "Node" << e->dest->tag; 
		}
		cout << endl;
	}
}
void Graph::remove_node(Node* node){
	//TODO
}