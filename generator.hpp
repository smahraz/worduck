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

class ConstPlaceHolder: public PlaceHolder{
	std::string str;
	public:
		ConstPlaceHolder(std::string str): str(str){}

		ConstPlaceHolder& operator++() override{
			cursor++;
			return *this;
		}

		operator std::string() const override{
			return str;
		}

		operator bool() override{
			return cursor == 0;
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
			this->end = end + 1;
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

// class GroupPlaceHolder: public PlaceHolder{
// 	std::vector<PlaceHolder *> ph;
// }

class Generator{
	std::vector<PlaceHolder*> placeHolders;


	public:
		~Generator(){
			for (PlaceHolder* p: placeHolders)
				delete p;
		}

		Generator(std::vector<PlaceHolder*> ph):placeHolders(ph){}

		void append(PlaceHolder* p){
			placeHolders.push_back(p);
		}

		std::string string() const{
			std::string output;
			for (const PlaceHolder* p: placeHolders){
				output += static_cast<std::string>(*p);
			}
			return output;
		}

		Generator& operator++(){
			for (int i = placeHolders.size() - 1; i >= 0; i--)
			{
				++(*placeHolders[i]);
				if (*placeHolders[i] == false)
				{
					if (i == 0)
						break;
					placeHolders[i]->resetCursor();
				}
				else{
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
