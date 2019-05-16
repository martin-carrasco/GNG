#include "input_generator.h"

IG::IG(const vector< pair<int, int> > vec){
    
	this->count =  vec.size();
    this->pos_vector = vec;

    /*for(auto current_pair : vec){
            pos_vector.push_back(current_pair);
	}*/
}
void IG::setInput(vector< pair<int,int> > vec){
    this->pos_vector = vec;
}
vector<pair<int, int>> IG::getInput(){
    return this->pos_vector;
}
unsigned long IG::size(){
	return pos_vector.size();
}

pair<int, int> IG::pop(){
    if(this->count < 0)
        this->count = this->pos_vector.size()-1;
	auto item = this->pos_vector[this->count];
    this->count--;
	return item;
}

pair<int, int> UIG::pop(){
	int gen =  dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
    pair<int, int> toReturn = this->pos_vector[gen];
	return toReturn;
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
