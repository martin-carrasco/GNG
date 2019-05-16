#ifndef GRAPH_NULL_INPUT_GENERATOR_H
#define GRAPH_NULL_INPUT_GENERATOR_H

#include <vector>
#include <random>

using namespace std;

class IG {
protected:
    int count;
    //Vectores de input que conforman la imagen
    vector<pair<int, int>> pos_vector;
public:
    IG(vector< pair<int, int> >  vec);

    virtual pair<int, int> pop();
    unsigned long size();
    vector< pair<int,int> > getInput();
    void setInput(vector< pair<int, int> > vec);
};

//Uniform Distribution
class UIG : public IG {
private:
	default_random_engine re;
	uniform_int_distribution<int> dist;
public:
	UIG(vector< pair<int, int> >  vec) : IG(vec){}
	virtual pair<int, int> pop();
};

//Moving Uniform Distribution
class MUIG : public IG {
private:
    default_random_engine re;
    uniform_int_distribution<int> dist;
    void moveXAxis();
    int direction = 1;
public:
    MUIG(vector< pair<int, int> >  vec) : IG(vec){}

    virtual pair<int, int> pop();
};

#include "input_generator.cpp"
#endif //GRAPH_NULL_GNG_H
