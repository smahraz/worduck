#include <cstddef>
#include <cstring>
#include <vector>
#include <string>

using u64 = unsigned long long;


struct start_end{
  size_t start;
  size_t end;
};

class PlaceHolder{
	protected:
		long cursor = 0;

	public:
		char* buff = nullptr;
		virtual operator bool() const = 0;
		virtual PlaceHolder& operator++() = 0;
		virtual size_t maxBuffSize() const = 0;
    virtual u64 comboCount() const = 0;

		virtual ~PlaceHolder(){
      delete[] buff;
    };

    virtual void reset(){
      cursor = 0;
    }

    static u64 power(u64 base, u64 exp){
      if (exp == 0)
        return 1;
      u64 result = base;
      while (exp - 1){
        result *= base;
        exp--;
      }
      return result;
    }
};

class RangePlaceHolder: public PlaceHolder{
  protected:
    unsigned int start;
    unsigned int end;
    char size;

    virtual void updateBuff(){
			strcpy(buff, std::to_string(start + cursor).c_str());
    }

	public:
		RangePlaceHolder(unsigned int start, unsigned int end){
			this->start = start;
			this->end = end;
			size = std::to_string(end).length();

			buff = new char[size + 1]();
      updateBuff();
		}

		virtual RangePlaceHolder& operator++() override{
			++cursor;
      updateBuff();
			return *this;
		}

		size_t maxBuffSize() const override{
      return size;
		}

		operator bool() const override{
			return start + cursor <= end;
		}

    u64 comboCount() const override{
      return end - start + 1;
    }
};

class PaddedRangePlaceHolder: public RangePlaceHolder{
  void updateBuff() override{
    std::string output = std::to_string(start + cursor);
    output = std::string(size - output.length(), '0') + output;
    strcpy(buff, output.c_str());
  }

  public:

    PaddedRangePlaceHolder(unsigned int start, unsigned int end): RangePlaceHolder(start, end){
      updateBuff();
    }
    PaddedRangePlaceHolder& operator++() override{
      ++cursor;
      std::string output = std::to_string(start + cursor);
      output = std::string(size - output.length(), '0') + output;
      strcpy(buff, output.c_str());
      return *this;
    }


};


class SetPlaceHolder: public PlaceHolder{
  std::vector<char> charSet;
  std::vector<short> indexSet;
  unsigned int size;

  public:
    SetPlaceHolder(std::vector<char> set, unsigned size = 1){
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

    u64 comboCount() const override{
      return power(charSet.size(), size); 
    }

};

class ConstPlaceHolder: public PlaceHolder{
  unsigned int size;

  public:
    ConstPlaceHolder(const std::string str){
      size = str.length();
      buff = new char[size + 1];
      strcpy(buff, str.c_str());
    }

    u64 comboCount() const override{
      return 1; 
    }

    operator bool() const override{
      return cursor == 0;
    }

    size_t maxBuffSize() const override{
      return size;
    }

    ConstPlaceHolder& operator++() override{
      cursor++;
      return *this;
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
			buff = new char[size + 1](0);
			
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
				++(*placeHolders[i]);
				if (*placeHolders[i] == false){
					if (i == 0)
						break;
					placeHolders[i]->reset();
				}
				else{
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



const std::vector<char> NUMBER_SET {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

const std::vector<char> LOWER_SET{
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z'
};

const std::vector<char> UPPER_SET{
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z'
};

const std::vector<char> ALPHANUMERIC_SET{
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z'
};

const std::vector<char> UPPER_LOWER_SET{
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
  'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
  'U', 'V', 'W', 'X', 'Y', 'Z'
};
