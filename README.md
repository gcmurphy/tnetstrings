## Overview

A rather bad implementation of [tnetstrings](http://tnetstrings.org/) for C++ 

(Still a work in progress)

## Creating a tnetstring

This library uses a simple builder pattern to allow for the construction 
of generic tnetstrings. 

```
tnetstring::Builder b;

b.append("foo");
b.append(453); 

std::cout << b.content() << std::endl;
// 3:foo,3:453#

```

You can currently append a map<string, string> or a vector<string>. For 
mixed content lists or dictionaries (or dictionaries of lists etc etc) 
you will need to use a slightly different API. 

```
// Create a list of random things

using namespace tnetstring;

Builder b; 

// A list of string, int, bool
b.enter()
 .append("This is a string")
 .append(123456)
 .append(true)
 .exit(LIST)

std::cout << b.content() << std::endl;
// 33:16:Thisisastring,6:123456#4:true!]

```

## Consuming a tnetstring primative types

```
std::string s = tnetstring::readString(std::cin);
float f = tnetstring::readFloat(std::cin);
bool b = tnetstring::readBool(std::cin);

// TODO (Provide more overloads to read directly from strings etc).
// Presently you would need to do this - 
std::string s = tnetstring::readString(std::stringstream("3:foo,"));

```

## Consuming a dictionary 

As per the specification:  A dictionary must use "strings" for keys only. 

``` 
tnetstring::Dictionary dict = tnetstring::readDictionary(std::cin);
std::string str =  tnetstring::toString(dict["this_key_has_a_string_value"]);
float f = tnetstring::toFloat(dict["this_key_has_a_float_value"]);

// TODO (Coming soon something along these lines) 
std::map<std::string, int> = tnetstring::readDictionaryOf<int>(std::cin, toInteger);


```


## Consuming a list

```
tnetstring::List items = tnetstring::readList(std::cin);
for (RawType item : items){
  // processing items on the fly
  std::cout << tnetstring::toString(item) << std::endl;
}

// TODO (Coming soon something along these lines) 
std::vector<int> items = tnetstring::readListOf<int>(std::cin, toInteger) 
```







