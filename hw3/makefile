CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Winit-self -Wshadow -Wlogical-op -Wcomment -Wctor-dtor-privacy -Wold-style-cast -D_GLIBCXX_DEBUG -fno-diagnostics-show-option

all: opts.cc
	$(CXX) $(CXXFLAGS) -ggdb opts.cc -o opts
	

clean:
	-rm -f opts *.o
