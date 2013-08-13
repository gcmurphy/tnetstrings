#include <gtest/gtest.h>
#include <tnetstring/tnetstring.hpp>
#include <map>
#include <sstream>
#include <cassert>


TEST(Types, readString){

  tnetstring::Builder b;    
  b.append("This is a test string");

  EXPECT_NE("", b.content());

  std::stringstream input(b.content());
  std::string content = tnetstring::readString(input);
  EXPECT_EQ(content,"This is a test string");
}

TEST(Types, readFloat){

  tnetstring::Builder b;    
  b.append(3.14159f);

  EXPECT_NE("", b.content());

  std::stringstream input(b.content());
  float pi = tnetstring::readFloat(input); 
  EXPECT_EQ(3.14159f, pi);
}


TEST(Types, readDictionary){

  tnetstring::Builder b;    
  std::map<std::string, std::string> dict;
  dict["key1"] = "value1";
  dict["key2"] = "value2";
  dict["key3"] = "value3";
  b.append(dict);

  EXPECT_NE("", b.content());

  std::stringstream input(b.content());
  tnetstring::Dictionary tdict = tnetstring::readDictionary(input);

  ASSERT_TRUE(tdict.find("key1") != tdict.end());
  EXPECT_EQ(tnetstring::toString(tdict["key1"]), "value1");

  ASSERT_TRUE(tdict.find("key2") != tdict.end());
  EXPECT_EQ(tnetstring::toString(tdict["key2"]), "value2");

  ASSERT_TRUE(tdict.find("key3") != tdict.end());
  EXPECT_EQ(tnetstring::toString(tdict["key3"]), "value3");

}

TEST(Types, readList){

  tnetstring::Builder b;    
  std::vector<std::string> list = { "Larry", "Moe", "Curly" };
  b.append(list);

  EXPECT_NE("", b.content());

  std::stringstream input(b.content());
  tnetstring::List tlist = tnetstring::readList(input);

  EXPECT_EQ(tnetstring::toString(tlist[0]), "Larry");
  EXPECT_EQ(tnetstring::toString(tlist[1]), "Moe");
  EXPECT_EQ(tnetstring::toString(tlist[2]), "Curly");

}



