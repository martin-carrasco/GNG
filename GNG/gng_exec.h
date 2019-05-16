
#ifndef GRAPH_NULL_GNG_EXEC
#define GRAPH_NULL_GNG_EXEC

#include "input_generator.h"
#include <vector>

using namespace std;

enum GenType{
   LINEAR,
   UNIFORM_DISTRIBUTION,
   MOVING_UNIFORM_DISTRIBUTION
};

template<template <class> class Algorithm, class Trait>
class GNGExec {
    bool is_running;
    IG* inpt_gen;
    Algorithm<Trait> *algorithm;
    GenType gen_type;

    public:
        GNGExec(Algorithm<Trait> *algorithm, vector<pair<int,int>> input_data, GenType gen_type=LINEAR, bool is_running = false)
        void next();
        void setInputData(vector< pair<int, int> > input_data);
        void setRunning(bool running);
        int getExecutionCount();
        bool isRunning();
        void draw();
};

#include "gng_exec.cpp"
#endif //GRAPH_NULL_GNG_EXEC
