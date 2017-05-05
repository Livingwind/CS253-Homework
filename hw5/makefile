CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option
EXE = test


all: U.o main.o
	$(CXX) $(CXXFLAGS) $^ -ggdb -o $(EXE)
	
U.o: U.cc
	$(CXX) $(CXXFLAGS) -ggdb -c $^

main.o: main.cc
	$(CXX) $(CXXFLAGS) $^ -ggdb -c $^

clean:
	-rm -f $(EXE) *.o
