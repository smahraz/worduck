#include "generator.hpp"
#include <iostream>
#include <vector>
#include <set>



void deleteArray(std::vector<PlaceHolder*> ph){
  for (PlaceHolder* p: ph)
    delete p;
}

PlaceHolder* convertSpecifiers(const std::string& str, int& i){
  PlaceHolder* placeHolder = nullptr;
  struct start_end param = {1, 1};
  switch (str[i]) {
    case 'd':
      placeHolder = new SetPlaceHolder(NUMBER_SET);
      break;
  }
  return placeHolder;
}

std::vector<PlaceHolder*> parser(std::string str){

  std::vector<PlaceHolder*> ph;
  std::string constPH;
  std::set<char> specifiers = {'d'};

  for (int i=0; i<str.length(); i++){
    if (str[i] != '%'){
      constPH += str[i];
      continue;
    }

    // incase of % at the end of string.
    if (i + 1 >= str.length())
    {
      deleteArray(ph);
      return {};
    }
    // the specifier is %%.
    if (str[i + 1] == '%')
    {
      constPH += '%';
      ph.push_back(new ConstPlaceHolder(constPH));
      constPH.clear();
      i++;
      continue;
    }
    if (constPH.length())
      ph.push_back(new ConstPlaceHolder(constPH));
    constPH.clear();
    if (!specifiers.contains(str[i + 1]))
    {
      deleteArray(ph);
      return {};
    }
    else{
      PlaceHolder* p = convertSpecifiers(str, ++i);
      if (p == nullptr){
        deleteArray(ph);
        return {};
      }
      ph.push_back(p);
    }
  }
    if (constPH.length())
      ph.push_back(new ConstPlaceHolder(constPH));
  return ph;
} 


int main(int argc, char** argv){

  if (argc != 2)
    return 1;

  auto ph = parser(argv[1]);
  
  if (ph.size() == 0)
    return 1;
	
	Generator g = Generator(ph);

	while (g)
	{
		std::cout<< g.buff << "\n";
		g++;
	}


	for (PlaceHolder* p: ph)
		delete p;

}
