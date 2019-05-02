
template<class GNGTrait>
unsigned long InputGenerator<GNGTrait>::size(){
	return pos_vector.size();
}

template <class GNGTrait>
sf::Vertex DefaultInputGenerator<GNGTrait>::pop(){
	auto item = this->pos_vector.back();
	this->pos_vector.pop_back();
	return item;	
}
template <class GNGTrait>
sf::Vertex UniformDistributionInputGenerator<GNGTrait>::pop(){
	int gen =  dist(re, uniform_int_distribution<int>::param_type(0, this->pos_vector.size()));
	sf::Vertex toReturn = this->pos_vector[gen];
	this->pos_vector.erase(this->pos_vector.begin()+gen);
	return toReturn;
}

template <class GNGTrait>
InputGenerator<GNGTrait>::InputGenerator(vector<sf::VertexArray> vec){
	for(auto vertArray : vec){
		for(int x = 0;x < vertArray.getVertexCount();x++){
			sf::Vertex v = vertArray[x];
			pos_vector.push_back(v);
		}
	}
}
