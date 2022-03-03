# CXX Make variable for compiler
CXX=g++
# # Make variable for compiler options
#std=c++11  C/C++ variant to use, e.g. C++ 2011
# #	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11 -g


#
# # Rules format:
# # target : dependency1 dependency2 ... dependencyN

# #     Command to make target, uses default rules if not specified
#
# # First target is the one executed if you just type make
# # make target specifies a specific target
# # $^ is an example of a special variable.  It substitutes all dependencies

dicttest: dicttest.o populatetree.o countwords.o
	$(CXX) $(CXXFLAGS) -o dicttest $^ -lpthread

populatetree.o: dicttree.h populatetree.cpp

countwords: dicttree.h countwords.cpp

dicttest.o: dicttree.h dicttest.cpp

clean :
	rm *.o
