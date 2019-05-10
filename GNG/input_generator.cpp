
template<class GNGTrait>
unsigned long InputGenerator<GNGTrait>::size(){
	return pos_vector.size();
}

template <class GNGTrait>
pair<int, int> DefaultInputGenerator<GNGTrait>::pop(){
    if(this->count < 0)
        this->count = this->pos_vector.size()-1;
	auto item = this->pos_vector[this->count];
    this->count--;
	return item;
}
template <class GNGTrait>
pair<int, int> UniformDistributionInputGenerator<GNGTrait>::pop(){
	int gen =  dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
	pair<int, int> toReturn = this->pos_vector[gen];
	return toReturn;
}
template <class GNGTrait>
void MovingUniformDistributionInputGenerator<GNGTrait>::moveXAxis() {
    for(auto element : this->pos_vector){
       if(element.first >= 640 - 10)
           direction *= -1;
       element.first = element.first + direction;
    }
}
template <class GNGTrait>
pair<int, int> MovingUniformDistributionInputGenerator<GNGTrait>::pop() {
    int gen = dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
    pair<int, int> toReturn = this->pos_vector[gen];
    moveXAxis();
    return toReturn;
}
template <class GNGTrait>
InputGenerator<GNGTrait>::InputGenerator(vector< vector< pair<int, int> > > vec){
	this->count = 0;
    for(auto current_vector : vec){
        this->count += current_vector.size();
		for(auto current_pair : current_vector){
            pos_vector.push_back(current_pair);
	    }
    }
}
