CXX = clang++
CXXFLAGS = -O3 -std=c++17 -Wall -g
INCLUDES = graph.hpp

test: test.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o test test.cpp

test: z2.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o z2 z2.cpp

clean:
	rm -f test z2 a.out
