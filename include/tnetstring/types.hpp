#ifndef tnetstring_types_h
#define tnetstring_types_h 

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>

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
      std::vector<char> buffer() const;

      friend std::ostream& operator << (std::ostream &os, const RawType &t);
  };

  template<typename T>
  class Type {
    private: 
      RawType raw;

    public: 
      explicit Type(RawType &t) : raw(t.size(), t.buffer(), t.type()){
      }
      T value(){
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
  T convert(RawType &r){
    if (r.type() != t){
      throw ConversionError(r.type(), t);
    }
    return Type<T>(r).value();
  }

  std::string toString(RawType &r);
  std::string readString(std::istream &input);
  int toInteger(RawType &r);
  int readInteger(std::istream &input);
  float toFloat(RawType &r);
  float readFloat(std::istream &input);
  bool toBoolean(RawType &r);
  bool readBoolean(std::istream &input);

  Dictionary toDictionary(RawType &r);
  Dictionary readDictionary(std::istream &input);
  List toList(RawType &r);
  List readList(std::istream &input);

  // TODO: Something along these lines..
  //template<typename T> std::vector<T> readListOf<T>(std::istream &input);
  //template<typename K, typename V> std::map<K, V> readMapOf(std::istream &input);

};

#endif // tnetstring_types_h
