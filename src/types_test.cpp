#include <gtest/gtest.h>
#include <tnetstring/tnetstring.hpp>
#include <map>
#include <sstream>
#include <cassert>
#include <functional>

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

  float sum = 0.0f; 
  tnetstring::Builder floats; 
  floats.enter().append(5.0f).append(1.0f).append(9.0f).exit(tnetstring::LIST);

  input.str(floats.content());

  tnetstring::List items = tnetstring::readList(input);
  for (auto item : items){
    sum += tnetstring::toFloat(item);
  }
  EXPECT_EQ(5.0f, sum/items.size());

}

TEST(Types, booleanValue){

  bool expectTrue = tnetstring::booleanValue("4:true!");
  bool expectFalse = tnetstring::booleanValue("5:false!");

  ASSERT_TRUE(expectTrue);
  ASSERT_FALSE(expectFalse);

}


TEST(Types, toListOf){
  tnetstring::Builder b; 
  b.enter().append(1).append(1).append(1).exit(tnetstring::LIST);
  tnetstring::List rawitems = tnetstring::listValue(b.content());

  std::vector<int> items = tnetstring::toListOf<int>(rawitems, tnetstring::toInteger);
  for (auto i : items){
    EXPECT_EQ(1, i);
  }
}


TEST(Types, toDictionaryOf){

  std::map<std::string, std::string> expected = {
    { "key1" , "val1"}, 
    { "key2" , "val2"}, 
    { "key3" , "val3"}
  };

  tnetstring::Builder b; 
  b.append(expected);

  tnetstring::Dictionary rawDict = tnetstring::dictionaryValue(b.content());
  std::map<std::string, std::string> actual = 
    tnetstring::toDictionaryOf<std::string>(rawDict, tnetstring::toString);

  for (auto &kv : actual){
    EXPECT_EQ(expected[kv.first], actual[kv.first]);
  }

}






