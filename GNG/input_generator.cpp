#include "input_generator.h"

<<<<<<< HEAD
IG::IG(const vector< pair<int, int> > vec){
    
	this->count =  vec.size();
    for(auto current_pair : vec){
            pos_vector.push_back(current_pair);
	}
}

IG::IG(const IG &input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
}
IG& IG::operator=(const IG& input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
    return *this;
}

vector<pair<int, int>> IG::getInput(){
    return this->pos_vector;
}
unsigned long IG::size(){
=======
template <class GNGTrait>
vector<pair<int, int>> InputGenerator<GNGTrait>::getInput(){
    return this->pos_vector;
}
template<class GNGTrait>
unsigned long InputGenerator<GNGTrait>::size(){
>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
	return pos_vector.size();
}

pair<int, int> IG::pop(){
    if(this->count < 0)
        this->count = this->pos_vector.size()-1;
	auto item = this->pos_vector[this->count];
    this->count--;
	return item;
}

UIG::UIG(const UIG &input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
    this->re = input_gen.re;
    this->dist = input_gen.dist;
}
UIG& UIG::operator=(const UIG &input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
    this->re = input_gen.re;
    this->dist = input_gen.dist;
    return *this;
}

pair<int, int> UIG::pop(){
	int gen =  dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
	pair<int, int> toReturn = this->pos_vector[gen];
	return toReturn;
}
MUIG::MUIG(const MUIG &input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
    this->direction = input_gen.direction;
    this->dist = input_gen.dist;
    this->re = input_gen.re;
}
MUIG& MUIG::operator=(const MUIG &input_gen){
    this->pos_vector = input_gen.pos_vector;
    this->count = input_gen.count;
    this->direction = input_gen.direction;
    this->dist = input_gen.dist;
    this->re = input_gen.re;
    return *this;
}

void MUIG::moveXAxis() {
    for(auto element : this->pos_vector){
       if(element.first >= 640 - 10)
           direction *= -1;
       element.first = element.first + direction;
    }
}
pair<int, int> MUIG::pop() {
    int gen = dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
    pair<int, int> toReturn = this->pos_vector[gen];
    moveXAxis();
    return toReturn;
}
<<<<<<< HEAD
=======
template <class GNGTrait>
InputGenerator<GNGTrait>::InputGenerator(vector< pair<int, int> > vec){
	this->count =  vec.size();
    for(auto current_pair : vec){
            pos_vector.push_back(current_pair);
	}
}

>>>>>>> 082b121e00b06a7aeeb6fc29a4d1c7d0e3dda46d
