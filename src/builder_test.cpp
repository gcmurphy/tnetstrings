#include <gtest/gtest.h>
#include <tnetstring/tnetstring.hpp>
#include <map>
#include <cassert>

/* TODO */
TEST(Builder, listTest){
  tnetstring::Builder b; 
  b.enter()
   .append("This is a string")
   .append(123456)
   .append(true)
   .exit(tnetstring::LIST);
    
  EXPECT_NE("", b.content());
}
