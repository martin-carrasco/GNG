#ifndef GRAPH_NULL_INPUT_GENERATOR_H
#define GRAPH_NULL_INPUT_GENERATOR_H

#include <random>

using namespace std;

template <class Trait>
class InputGenerator{
protected:
    typedef ::Node<Graph<Trait>>* NodePtr;
    //Vectores de input que conforman la imagen
    vector<sf::Vertex> pos_vector;
public:
    InputGenerator(vector<sf::VertexArray> vec);
    virtual sf::Vertex pop() = 0;
    unsigned long size();

};

template <class Trait>
class UniformDistributionInputGenerator : public InputGenerator<Trait>{
private:
	default_random_engine re;
	uniform_int_distribution<int> dist;
public:
	UniformDistributionInputGenerator(vector<sf::VertexArray> vec) : InputGenerator<Trait>(vec) {}
	sf::Vertex pop();
};

template <class Trait>
class DefaultInputGenerator : public InputGenerator<Trait>{
public:
	DefaultInputGenerator(vector<sf::VertexArray> vec) : InputGenerator<Trait>(vec) {}
	sf::Vertex pop();
};

#include "input_generator.cpp"
#endif //GRAPH_NULL_GNG_H
