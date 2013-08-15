#ifndef tnetstring_types_h
#define tnetstring_types_h 

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <functional>

namespace tnetstring {

  enum TYPE {
    STRING    = ',', 
    INTEGER   = '#', 
    FLOAT     = '^', 
    BOOLEAN   = '!', 
    LIST      = ']', 
    DICTIONARY= '}',
    NULL_PTR  = '~'
  };

  static std::map<TYPE, std::string> TYPE_NAME = {
    { BOOLEAN , "boolean" }, 
    { STRING  , "string"  }, 
    { INTEGER , "integer" }, 
    { FLOAT   , "float"   }, 
    { LIST    , "list"    },
    { DICTIONARY, "dictionary" }, 
    { NULL_PTR, "null" }
  };

  class InputError : public std::exception {
    private:
      std::string error;
    public:
      InputError(char invalid, size_t pos);
      const char *what() const throw();
  };

  class ConversionError : public std::exception {
    private:
      std::string error;
    public:
      ConversionError(TYPE actual, TYPE expected);
      const char *what() const throw();
  };

  class InvalidInputError : public std::exception { 
     private:
      std::string error;
    public:
      InvalidInputError(const std::string &error);
      const char *what() const throw();
  };

  class RawType {

    private:
      size_t dataSize;
      std::vector<char> data;
      TYPE dataType;

    public: 
      RawType();
      RawType(std::istream &input);
      RawType(size_t sz, const std::vector<char> &buf,  TYPE t);
      void copy(std::ostream &out) const ;
      size_t size() const ;
      TYPE type() const ;
      const std::vector<char>& buffer() const;

      friend std::ostream& operator << (std::ostream &os, const RawType &t);
  };

  template<typename T>
  class Type {
    private: 
      RawType raw;

    public: 
      explicit Type(const RawType &t) : raw(t.size(), t.buffer(), t.type()){
      }
      T value() const {
        T val;
        std::stringstream tmp;
        raw.copy(tmp);
        tmp >> val;
        if (!tmp.eof()){
          char c = tmp.get();
          tmp.unget();
          throw InputError(c, tmp.tellg());
        }
        return val;
      }
  };

  using Dictionary = std::map<std::string, RawType>;
  using List = std::vector<RawType>;

  template<typename T, TYPE t>
  T convert(const RawType &r){
    if (r.type() != t){
      throw ConversionError(r.type(), t);
    }
    return Type<T>(r).value();
  }

  std::string toString(const RawType &r);
  std::string stringValue(const std::string &input);
  std::string readString(std::istream &input);

  int toInteger(const RawType &r);
  int integerValue(const std::string &input);
  int readInteger(std::istream &input);

  float toFloat(const RawType &r);
  float floatValue(const std::string &input);
  float readFloat(std::istream &input);

  bool toBoolean(const RawType &r);
  bool booleanValue(const std::string &input);
  bool readBoolean(std::istream &input);

  Dictionary toDictionary(const RawType &r);
  Dictionary dictionaryValue(const std::string &input);
  Dictionary readDictionary(std::istream &input);

  List toList(const RawType &r);
  List listValue(const std::string &input);
  List readList(std::istream &input);

  template<typename T> 
  std::vector<T> toListOf(const List &items, 
    std::function<T(const RawType &r)> converter){

    std::vector<T> converted;
    for (auto item : items){
      converted.push_back(converter(item));
    }
    return converted;
  }

  template<typename T>
  std::map<std::string, T> toDictionaryOf(const Dictionary &dict, 
    std::function<T(const RawType &r)> converter){

    std::map<std::string, T> converted;
    for (const auto& entry : dict) {
      converted[entry.first] = converter(entry.second);
    }
    return converted;
  }

};

#endif // tnetstring_types_h
