SRCS = tests/main.cpp src/graphs.cpp
FLAGS = --std=c++11 -lgtest -lpthread -I./include/

.PHONY: test.out

test.out : $(SRCS) include/graphs.hpp
	g++ $(SRCS) $(FLAGS) -o test.out