#include <tnetstring/builder.hpp>
#include <algorithm>
#include <iterator>
#include <iostream>


namespace tnetstring {

void Builder::clearBuffer(){
  buffer.str("");  
  buffer.clear();
}

Builder &Builder::enter(){
  nested.push(buffer.str());
  clearBuffer();
  return *this;
}

Builder &Builder::exit(TYPE type){
  //std::string tmp = buffer.str();
  std::vector<char> data;
  std::copy(std::istream_iterator<char>(buffer), 
      std::istream_iterator<char>(), 
      std::back_inserter(data));

  RawType raw(data.size(), data, type);

  clearBuffer();
  if (nested.empty()){
    buffer << std::noskipws << raw;

  } else {
    
    buffer << std::noskipws << nested.top() << raw;
    nested.pop();
  }

  return *this;
}

std::string Builder::content(){
  if (! nested.empty()){
    throw tnetstring::InvalidInputError("Unterminated & nested content");
  }
  return buffer.str();
}

Builder &Builder::append(int i){
  std::vector<char> data;
  std::stringstream ss; 
  ss << i; 
  std::copy(std::istream_iterator<char>(ss), 
      std::istream_iterator<char>(), 
      std::back_inserter(data));

  RawType r(data.size(), data, INTEGER);
  buffer << r;
  return *this;
}

Builder &Builder::append(float f){

  std::vector<char> data;
  std::stringstream ss; 
  ss << f; 
  std::copy(std::istream_iterator<char>(ss), 
      std::istream_iterator<char>(), 
      std::back_inserter(data));

  RawType r(data.size(), data, FLOAT);
  buffer << r;
  return *this;
}

Builder &Builder::append(const char *cstr){
  append(std::string(cstr));
  return *this;
}

Builder &Builder::append(const std::string &str){
  std::vector<char> data;
  for (char c : str){
    data.push_back(c);
  }
  RawType r(data.size(), data, STRING);
  buffer << std::noskipws << r;
  return *this;
}

Builder &Builder::append(bool b){
  std::string bstr = (b) ? "true" : "false"; 
  std::vector<char> data;
  for (char c : bstr){
    data.push_back(c);
  }
  RawType r(data.size(), data, BOOLEAN);
  buffer << r;
  return *this;
}

Builder &Builder::append(const std::vector<std::string> &l){
  enter();
  for (auto iter = l.cbegin(); iter != l.cend(); iter++){
    append(*iter);
  }
  exit(LIST);
  return *this;
}

Builder &Builder::append(const std::map<std::string, std::string> &properties){
  enter();
  for(auto iter = properties.begin(); iter != properties.end(); iter++){
    append(iter->first);
    append(iter->second);
  }
  exit(DICTIONARY);
  return *this;
}

Builder &Builder::append_nullptr(){
  std::vector<char> data(0);
  RawType r(0, data, NULL_PTR);
  buffer << r;
  return *this;
}

}; // tnetstring
