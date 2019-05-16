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
    IG(){}
    IG(const vector< pair<int, int> >  vec);
    IG(const IG &input_gen);
    IG& operator=(const IG &input_gen);

    virtual pair<int, int> pop();
    unsigned long size();
    vector<pair<int,int>> getInput();
};

//Uniform Distribution
class UIG : public IG {
private:
	default_random_engine re;
	uniform_int_distribution<int> dist;
public:
	UIG(vector< pair<int, int> >  vec) : IG(vec){}
    UIG(const UIG &input_gen);
    UIG& operator=(const UIG &input_gen);

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
    MUIG(const MUIG &input_gen);
    MUIG& operator=(const MUIG &input_gen);

    virtual pair<int, int> pop();

};

#include "input_generator.cpp"
#endif //GRAPH_NULL_GNG_H
