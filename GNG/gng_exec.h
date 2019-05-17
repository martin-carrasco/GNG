
#ifndef GRAPH_NULL_GNG_EXEC
#define GRAPH_NULL_GNG_EXEC

#include "input_generator.h"
#include <vector>
#include "../graph.h"

using namespace std;

enum GenType{
   LINEAR,
   UNIFORM_DISTRIBUTION,
   MOVING_UNIFORM_DISTRIBUTION
};

template<template <class> class Algorithm, class Trait>
class GNGExec {
    typedef ::Node<Graph<Trait>>* NodePtr;
    typedef ::Edge<Graph<Trait>>* EdgePtr;
    bool is_running;
    IG* inpt_gen;
    Algorithm<Trait>* algorithm;
    GenType gen_type;
    unsigned int width, height;
    public:
        GNGExec(pair<int, int> dimentions, vector<pair<int,int>> input_data, GenType gen_type=LINEAR, bool is_running = false){
 
            this->width = dimentions.first;
            this->height = dimentions.second;
            this->algorithm = new Algorithm<Trait>(width, height);
            this->algorithm->init();

            switch(gen_type)
            {
                case UNIFORM_DISTRIBUTION:
                    inpt_gen = new UIG(input_data);
                    break;
                case MOVING_UNIFORM_DISTRIBUTION:
                    inpt_gen = new MUIG(input_data);
                    break;
                default:
                    gen_type = LINEAR;
                    inpt_gen = new IG(input_data);
                    break;
            }
            gen_type = gen_type;
        }
      
        auto getNodes(){
            return this->algorithm->get_graph().getNodesVector();
        }
        void next();
        void setInputData(vector< pair<int, int> > input_data);
        int getExecutionCount();

        void setRunning(bool running);
        bool isRunning();

};

#include "gng_exec.cpp"
#endif //GRAPH_NULL_GNG_EXEC
