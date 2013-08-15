#include <tnetstring/types.hpp>

namespace tnetstring {

//
// Errors 
//
InputError::InputError(char invalid, size_t pos){
  std::stringstream e;
  e << "Unexpected character `" << invalid << "` at position " << pos; 
  error = e.str();
}

const char *InputError::what() const throw() { 
  return error.c_str(); 
}

ConversionError::ConversionError(TYPE actual, TYPE expected){
  std::stringstream e;
  e << "Invalid type conversion to `"
    << TYPE_NAME[actual]
    << "` expected `"
    << TYPE_NAME[expected]
    << "`";
  error = e.str();
};

const char *ConversionError::what() const throw(){ 
  return error.c_str(); 
}  

InvalidInputError::InvalidInputError(const std::string &e) : error(e) {
}

const char *InvalidInputError::what() const throw(){
  return error.c_str();
}


// 
// Raw Type implementation
//

RawType::RawType() : dataType(NULL_PTR){
}

RawType::RawType(size_t sz, const std::vector<char> &buf,  TYPE t) 
  : dataSize(sz)
  , data(buf)
  , dataType(t){
}  


RawType::RawType(std::istream &input){
  input >> dataSize;
  char colon;
  input.read(&colon, 1);
  if (':' != colon){
    throw InputError(colon, input.tellg());
  }
  data.resize(dataSize);
  input.read(&data[0], dataSize);
  char sym;
  input >> sym;
  dataType = static_cast<TYPE>(sym);
}

void RawType::copy(std::ostream &out) const {
  for (char c : data){
    out << c;
  }
}

size_t RawType::size() const {
  return dataSize;
}

TYPE RawType::type() const{ 
  return dataType;
}

const std::vector<char> &RawType::buffer() const {
  return data;
}

std::ostream& operator<<(std::ostream &out, const RawType &raw) {
  out << raw.size() << ":"; 
  raw.copy(out);
  out << static_cast<char>(raw.type());
  return out;
}

// 
// Template specialization
//

template<>
class Type<std::string> {
  private:
    RawType raw;
  public:
    explicit Type(const RawType &t) : raw(t.size(), t.buffer(), t.type()){
    }
    std::string value() const {
      std::stringstream tmp;
      raw.copy(tmp);
      return tmp.str();
    }
};


template<>
class Type<bool> {
  private:
    RawType raw;
  public:
    explicit Type(const RawType &t) : raw(t.size(), t.buffer(), t.type()){
    }

    bool value() const {
      std::string boolString;
      std::stringstream tmp;
      raw.copy(tmp);
      tmp >> boolString;

      if (!(boolString  == "true" || boolString == "false")){
        throw ConversionError(raw.type(), BOOLEAN);
      }
      return boolString == "true";
    }
};

template<>
class Type<List>{
  private:
    RawType raw;
  public:
    explicit Type(const RawType &t) : raw(t.size(), t.buffer(), t.type()){
    }
    List value() const {
      if (raw.type() != LIST){
        throw ConversionError(raw.type(), LIST); 
      }

      List list;
      std::stringstream tmp; 
      raw.copy(tmp);
      long len = tmp.str().size();
      while (tmp.tellg() < len){
        RawType item(tmp);
        list.push_back(item);
      }
      return list;
    }
};

template<>
class Type<Dictionary>{
  private:
    RawType raw;
  public:
    explicit Type(const RawType &t) : raw(t.size(), t.buffer(), t.type()){
    }

    Dictionary value() const {
      if (raw.type() != DICTIONARY){
        throw ConversionError(raw.type(), DICTIONARY);
      }

      Dictionary dict;
      std::stringstream tmp; 
      raw.copy(tmp);

      long len = tmp.str().size();
      while (tmp.tellg() < len){
        RawType key(tmp);
        RawType val(tmp);
        if (key.type() != STRING){
          throw ConversionError(key.type(), STRING);
        }
        dict[Type<std::string>(key).value()] = val;
      }
      return dict;
    }
};

std::string toString(const RawType &r){
  return convert<std::string, STRING>(r);
}

std::string stringValue(const std::string &str){
  std::stringstream ss(str);
  return readString(ss);
}

std::string readString(std::istream &input){
  RawType raw(input);
  return toString(raw);
}

int toInteger(const RawType &r){
  return convert<int, INTEGER>(r);
}

int integerValue(const std::string &str){
  std::stringstream ss(str);
  return readInteger(ss);
}

int readInteger(std::istream &input){
  RawType raw(input);
  return toInteger(raw);
}

float toFloat(const RawType &r){
  return convert<float, FLOAT>(r);
}

float floatValue(const std::string &str){
  std::stringstream ss(str);
  return readFloat(ss);
}

float readFloat(std::istream &input){
  RawType raw(input);
  return toFloat(raw);
}

bool toBoolean(const RawType &r){
  return convert<bool, BOOLEAN>(r);
}

bool booleanValue(const std::string &str){
  std::stringstream ss(str);
  return readBoolean(ss);
}

bool readBoolean(std::istream &input){
  RawType raw(input);
  return toBoolean(raw); 
}

Dictionary toDictionary(const RawType &r){
  return convert<Dictionary, DICTIONARY>(r);
}

Dictionary dictionaryValue(const std::string &str){
  std::stringstream ss(str);
  return readDictionary(ss);
}

Dictionary readDictionary(std::istream &input){
  RawType raw(input);
  return toDictionary(raw);
}

List toList(const RawType &r){
  return convert<List, LIST>(r);
}

List listValue(const std::string &str){
  std::stringstream ss(str);
  return readList(ss);
}

List readList(std::istream &input){
  RawType raw(input);
  return toList(raw);
}

}; // tnetstring
