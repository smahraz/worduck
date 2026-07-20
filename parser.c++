#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_set>
#include <cstring>
#include <sstream>
#include "generator.hpp"


static std::unordered_set<char> validSpecifier = {
	'd', // [0-9]
	's', // set
	'r', // range
	'w', // [a-zA-Z0-9]
	'f', // file
	'%', // % itself
};


void freePlaceHolderVector(std::vector<PlaceHolder*> arr){
	for (PlaceHolder* p: arr)
		delete p;
}

PlaceHolder* getPlaceHolder(std::string s){
	switch (s[0]) {
		case 'd':
			if (s.size() > 1 && s[1] == '{'){
				return NULL;
			}
			return new RangePlaceHolder(0, 9);
	}
	return NULL;
}


std::vector<PlaceHolder*> parse(std::string s){
	std::string fixPlaceHolder;
	std::vector<PlaceHolder *> frame;

	for (unsigned int i = 0; i < s.length(); i++){
		if (s[i] != '%'){
			fixPlaceHolder += s[i];
			continue;
		}
		i++; // skip %
		if (fixPlaceHolder.length()){
			frame.push_back(new ConstPlaceHolder(fixPlaceHolder));
			fixPlaceHolder.clear();
		}
		if (i == s.length() || validSpecifier.find(s[i]) == validSpecifier.end()){
			freePlaceHolderVector(frame);
			std::cerr << "Unknown specifier after %";
			if (i < s.length())
				std::cerr << s.at(i);
			std::cerr << std::endl;
			return {};
		}
		PlaceHolder* ph = getPlaceHolder(s.substr(i));
		if (ph == NULL)
		{
			freePlaceHolderVector(frame);
			return {};
		}
		frame.push_back(ph);
	}
	if (!fixPlaceHolder.empty())
		frame.push_back(new ConstPlaceHolder(fixPlaceHolder));
	return frame;
}


int main(int argc, char **argv){
	if (argc != 2){
		std::cerr << "Wrong number of arguments\n";
		return 1;
	}

	std::vector<PlaceHolder*> placeHolders = parse(argv[1]);
	if (placeHolders.size() == 0)
		return 1;
	Generator g(placeHolders);

	while (g){
		std::cout << g.string() << "\n";
		++g;
	}
}
