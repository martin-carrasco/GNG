#include <iostream>
#include "graph.cpp"

using namespace std;


int main(){
 	Node* n = new Node();
 	Node* n2 = new Node();
	Node* n3 = new Node();
 	Node* n4 = new Node();
 	Node* n5 = new Node();

 	n->tag = '1';
 	n2->tag='2';
 	n3->tag='3';
 	n4->tag='4';
 	n5->tag='5';

 	Graph grafo;
 	
 	grafo.add_node(n);
 	grafo.add_node(n2);
 	grafo.add_node(n3);
 	grafo.add_node(n4);
 	grafo.add_node(n5);
 	
 	grafo.add_connection(n,n2,2);
 	grafo.add_connection(n2, n3, 3);
 	grafo.add_connection(n3, n4, 4);
 	grafo.add_connection(n4, n5, 2);
 	grafo.add_connection(n5, n, 2);

 	grafo.describe();

 	cout << endl;

 	for(auto gs : grafo.dfs(n)){
 		cout << gs->tag << " ";
 	}


	return 0;

}