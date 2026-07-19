#include <vector>
#include <sstream>
#include <iostream>
#include <string>

class PlaceHolder{
	protected:
		long cursor = 0;


	public:
		virtual operator std::string() const = 0;
		virtual operator bool() = 0;
		virtual PlaceHolder& operator++() = 0;

		virtual ~PlaceHolder() = default;

		void resetCursor(){
			cursor = 0;
		}
};

template <typename T>
class SetPlaceHolder: public PlaceHolder{
	std::vector<T> set;
	int max;


	public:
		SetPlaceHolder(std::vector<T> set): set(set){
			max = set.size();
		}

		SetPlaceHolder& operator++() override{
			++cursor;
			return *this;
		}

		operator std::string() const override{
			return {set[cursor]};
		}

		operator bool() override{
			return cursor < max;
		}

};

class RangePlaceHolder: public PlaceHolder{
	unsigned int start;
	unsigned int end;

	public:
		RangePlaceHolder(unsigned int start, unsigned int end){
			this->start = start;
			this->end = end;
		}

		RangePlaceHolder& operator++() override{
			++cursor;
			return *this;
		}

		operator std::string() const override{
			return std::to_string(cursor + start);
		}

		operator bool() override{
			return start + cursor < end;
		}
};

class Generator{
	std::vector<PlaceHolder*> placeHolders;


	public:
		~Generator(){
			for (PlaceHolder* p: placeHolders)
				delete p;
		}

		void append(PlaceHolder* p){
			placeHolders.push_back(p);
		}

		std::string string() const{
			std::stringstream ss;
			for (const PlaceHolder* p: placeHolders){
				ss << static_cast<std::string>(*p);
			}
			return ss.str();
		}

		Generator& operator++(){
			for (int i = placeHolders.size() - 1; i >= 0; i--)
			{
				if (*placeHolders[i] == false)
				{
					if (i == 0)
						break;
					placeHolders[i]->resetCursor();
				}
				else{
					++(*placeHolders[i]);
					break;
				}
			}
			return *this;
		}

		operator bool() const {
			if (placeHolders.size() == 0)
				return false;
			return *placeHolders[0];
		}
};



int main(){
	Generator gn;
	
	gn.append(new SetPlaceHolder<std::string>({"alex", "jacob", "eren"}));
	gn.append(new RangePlaceHolder(2000, 2026));

	while (gn){
		std::cout << gn.string() << std::endl;
		++gn;
	}
}
