#include "gng_exec.h"

template<template <class> class Algorithm, class Trait>
void GNGExec<Algorithm, Trait>::next(){

    pair<int, int> input_point = inpt_gen.pop();
    this->algorithm->exec(input_point);
}
template< template <class> class Algorithm, class Trait>
void GNGExec<Algorithm, Trait>::setRunning(bool running){
    this->is_running = running;
}
template< template <class> class Algorithm, class Trait>
bool GNGExec<Algorithm, Trait>::isRunning(){
    return this->is_running;
}
template< template <class> class Algorithm, class Trait>
int GNGExec<Algorithm, Trait>::getExecutionCount(){
    return this->algorithm->get_iteracion();
}
template< template <class> class Algorithm, class Trait>
void GNGExec<Algorithm, Trait>::setInputData(vector< pair<int, int> > vec){ 
    switch(this->gen_type)
    {
        case UNIFORM_DISTRIBUTION:
            inpt_gen = UIG(vec);
            break;
        case MOVING_UNIFORM_DISTRIBUTION:
            inpt_gen = MUIG(vec);
            break;
        default:
            inpt_gen = IG(vec);
            break;
    }
}
