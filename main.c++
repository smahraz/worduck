#include "generator.hpp"
#include <vector>
#include <iostream>

#define NUMBER_SET {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}


int main(void){
	
	std::vector<PlaceHolder*> placeHolders = {
		// new RangePlaceHolder(1, 100),
		// new RangePlaceHolder(1, 100),
    new SetPlaceHolder(NUMBER_SET, 2),
    new ConstPlaceHolder("Alex"),
    new SetPlaceHolder(NUMBER_SET, 1),
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
