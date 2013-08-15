from glob import glob
import os
import sys

warnings = [
    "-Wall",
    "-Werror",
    "-Wextra",
    "-Wold-style-cast",
    "-Wsign-promo"
]

include_dirs = [
  "-I.",
  "-I./include",
  "-I./gtest-1.6.0/include"
]

defines = [
  "GTEST_USE_OWN_TR1_TUPLE"
]

cflags = [
  "-std=c++11",
  "-g"
]

cflags.extend(warnings)
cflags.extend(include_dirs)
env = Environment()
env['ENV']['TERM'] = os.environ['TERM']
env.Append(CPPDEFINES=defines)
if sys.platform == "darwin" : 
  env.Replace(CXX='g++-4.8')
else:
  env.Replace(CXX='clang++')
env.Append(CCFLAGS=cflags)

srcs = [
    "src/types.cpp",
    "src/builder.cpp"
]

libs = [
]

libtnetstring = env.SharedLibrary('tnetstring', srcs, LIBS=libs)

testing_enabled = True 
if testing_enabled : 
    testlibs = [libtnetstring, 'gtest', 'gtest_main']
    testsrcs = ['src/unittests.cpp']
    testsrcs.extend(glob("src/*_test.cpp"))
    testmain = env.Program("test", testsrcs, LIBS=testlibs, LIBPATH="./gtest-1.6.0")
    tests = Alias("test", [testmain])
    AlwaysBuild(tests)

