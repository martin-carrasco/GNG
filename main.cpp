#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <queue>
#include <assert.h>
#include "graph.h"
#include "MST/prim.h"

using namespace std;


int main(int argc, char *argv[]) {
	Graph<CustomTrait> graph;

	graph.insert_node("A");
	graph.insert_node("B");
	graph.insert_node("C");
    graph.insert_node("D");
    graph.insert_node("E");
    graph.insert_node("F");
    graph.insert_node("Z");
    graph.insert_node("X");

	graph.insert_edge(1, "A", "C", false);
	graph.insert_edge(1, "A", "B", false);
    graph.insert_edge(1, "B", "D", false);
    graph.insert_edge(1, "C", "F", false);
    graph.insert_edge(1, "C", "E", false);
    graph.insert_edge(1, "Z", "X", false);


    //graph.delete_edge("C", "B", 10);
	//graph.delete_edge("C", "A", 5);
	//graph.delete_node("B");


	Prim<CustomTrait> prim(graph);
	Prim<CustomTrait> min_span = prim.generate_minimum_spanning_tree();

	graph.describe_helper(graph.bfs("A"));
	//graph.describe();


	return 0;
}
