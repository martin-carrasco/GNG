#include "gng_exec.h"

template<template <class> class Algorithm, class Trait>
GNGExec<Algorithm, Trait>::GNGExec<Algorithm, Trait>(Algorithm<Trait> *algorithm, vector<pair<int,int>> input_data, GenType gen_type=LINEAR, bool is_running = false){

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
    this->algorithm = algorithm;
    gen_type = gen_type;
}
template<template <class> class Algorithm, class Trait>
void GNGExec<Algorithm, Trait>::next(){

    if(!is_running)
        return;

    pair<int, int> input_point = inpt_gen->pop();
    cout << input_point.first << ", " << input_point.second << endl;
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
    inpt_gen->setInput(vec);
}
template< template <class> class Algorithm, class Trait>
void GNGExec<Algorithm, Trait>::draw(){
    this->drawExtras(this->algo->get_graph(), this->algo->get_iteracion(), ); //TODO figure out how to pass images
    this->drawImage();
}
