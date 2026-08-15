#include <cstddef>
#include <cstring>
#include <strings.h>
#include <vector>
#include <string>


class PlaceHolder{
	protected:
		long cursor = 0;


	public:
		char* buff = nullptr;
		virtual operator bool() const = 0;
		virtual PlaceHolder& operator++() = 0;
		virtual size_t maxBuffSize() const = 0;
    virtual void reset() = 0;

		virtual ~PlaceHolder() = default;
};

class RangePlaceHolder: public PlaceHolder{
	unsigned int start;
	unsigned int end;

	public:
		RangePlaceHolder(unsigned int start, unsigned int end){
			this->start = start;
			this->end = end + 1;

			size_t buffSize = std::to_string(end).length() + 1;
			buff = new char[buffSize]();
			strcpy(buff, std::to_string(start + cursor).c_str());
		}

		virtual RangePlaceHolder& operator++() override{
			++cursor;
			strcpy(buff, std::to_string(start + cursor).c_str());
			return *this;
		}

		size_t maxBuffSize() const override{
			return std::to_string(end).length();
		}

		operator bool() const override{
			return start + cursor < end;
		}

		~RangePlaceHolder()
		{
			delete[] buff;
		}

    void reset() override{
      cursor = 0;
    }
};

class SetPlaceHolder: public PlaceHolder{
  std::vector<char> charSet;
  std::vector<short> indexSet;
  unsigned int size;

  public:
    SetPlaceHolder(std::vector<char> set): charSet(set){}

    SetPlaceHolder(std::vector<char> set, unsigned size){
      charSet = set;
      this->size = size;
      buff = new char[size + 1];
      memset(buff, charSet[0], size);
      buff[size] = '\0';
      indexSet = std::vector<short>(size, 0);
    }

    SetPlaceHolder& operator++() override{
       for (int i= size - 1; i >= 0; i--){
         indexSet[i]++;
         if (indexSet[i] >= charSet.size()){
           if (i == 0)
             break;
           indexSet[i] = 0;
           buff[i] = charSet[0];
           continue;
         }
         else{
           buff[i] = charSet[indexSet[i]];
         }
         break;
       }
       return *this;
    } 

    size_t maxBuffSize() const override{
      return size;
    }

    void reset() override{
      indexSet = std::vector<short>(size, 0);
      memset(buff, charSet[0], size);
    }

    operator bool() const override{
      return indexSet[0] < charSet.size();
    }

    ~SetPlaceHolder(){
      delete[] buff;
    }

};

class Generator{
	std::vector<PlaceHolder*> placeHolders;

	void updateBuff() const{
		buff[0] = '\0';
		for (const PlaceHolder* p: placeHolders)
			strcat(buff, p->buff);
	}

	public:
		char *buff = nullptr;

		Generator(std::vector<PlaceHolder*> placeHolders): placeHolders(placeHolders){
			size_t size = 0;
			for (const PlaceHolder* p: placeHolders)
				size += p->maxBuffSize();
			buff = new char[size]();
			
			if (buff == nullptr)
				return;
			for (PlaceHolder* p: placeHolders)
				strcat(buff, p->buff);
		}

		operator bool () const {
			return static_cast<bool>(*placeHolders[0]);
		}

		size_t operator++(int){
			// current place holder.
			for (int i = placeHolders.size() - 1; i >= 0; i--)
			{
				if (*placeHolders[i] == false){
					if (i == 0)
						break;
					placeHolders[i]->reset();
				}
				else{
					++(*placeHolders[i]);
					updateBuff();
					break;
				}
			}
			return 0;
		}

		~Generator(){
			delete[] buff;
		}
};
