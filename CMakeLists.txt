cmake_minimum_required (VERSION 2.6)
project (graph-null)
set (CMAKE_CXX_STANDARD 17)
add_executable(graph-null main.cpp graph.h MST/prim.h MST/kruskal.h GNG/gng.cpp)