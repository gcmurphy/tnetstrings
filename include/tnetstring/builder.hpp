// Copyright 2015 Grant Murphy. All rights reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef tnetstring_builder_h
#define tnetstring_builder_h 

#include <tnetstring/types.hpp>
#include <stack>

namespace tnetstring {
  
  class Builder {
  
    private:
      std::stack<std::string> nested; 
      std::stringstream buffer;
      void clearBuffer();
    
    public:
      Builder &enter();
      Builder &exit(TYPE t=DICTIONARY);
      std::string content();
      Builder &append(int i);
      Builder &append(float f);
      Builder &append(const std::string &str);
      Builder &append(const char *c_str);
      Builder &append(bool b);
      Builder &append(const std::map<std::string, std::string> &properties);
      Builder &append(const std::vector<std::string> &items);
      Builder &append_nullptr();
  };
};

#endif // tnetstring_builder_h
