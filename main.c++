#include "generator.hpp"
#include <vector>
#include <iostream>

int main(void){
	
	std::vector<PlaceHolder*> placeHolders = {
		new RangePlaceHolder(1, 100),
	};


	Generator g = Generator(placeHolders);

	while (g)
	{
		std::cout<< g.buff << "\n";
		g++;
	}


	for (PlaceHolder* p: placeHolders)
		delete p;

}
