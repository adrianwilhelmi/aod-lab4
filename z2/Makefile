CXX = clang++
CXXFLAGS = -O3 -std=c++17 -Wall -g
INCLUDES = graph.hpp

test: test.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o test test.cpp

clean:
	rm -f test a.out
