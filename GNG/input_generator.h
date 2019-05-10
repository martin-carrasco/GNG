#ifndef GRAPH_NULL_INPUT_GENERATOR_H
#define GRAPH_NULL_INPUT_GENERATOR_H

#include <random>

using namespace std;

template <class Trait>
class InputGenerator{
protected:
    int count;
    typedef ::Node<Graph<Trait>>* NodePtr;
    //Vectores de input que conforman la imagen
    vector<pair<int, int>> pos_vector;
public:
    InputGenerator(vector< vector< pair<int, int> > > vec);
    virtual pair<int, int> pop() = 0;
    unsigned long size();

};
template <class Trait>
class MovingUniformDistributionInputGenerator : public InputGenerator<Trait> {
private:
    default_random_engine re;
    uniform_int_distribution<int> dist;
    void moveXAxis();
    int direction = 1;
public:
    MovingUniformDistributionInputGenerator(vector< vector< pair<int, int> > > vec) : InputGenerator<Trait>(vec){}
    virtual pair<int, int> pop();
};
template <class Trait>
class UniformDistributionInputGenerator : public InputGenerator<Trait>{
private:
	default_random_engine re;
	uniform_int_distribution<int> dist;
public:
	UniformDistributionInputGenerator(vector< vector< pair<int, int> > > vec) : InputGenerator<Trait>(vec) {}
	virtual pair<int, int> pop();
};

template <class Trait>
class DefaultInputGenerator : public InputGenerator<Trait>{
public:
    DefaultInputGenerator(vector< vector< pair<int, int> > > vec) : InputGenerator<Trait>(vec) {}
	virtual pair<int, int> pop();
};

#include "input_generator.cpp"
#endif //GRAPH_NULL_GNG_H
