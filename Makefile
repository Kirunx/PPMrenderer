CFLAGS = -std=c++17 -O2


PPMrndr: *.hpp *.cpp
	g++ $(CFLAGS) -o PPMrndr *.cpp 

test: PPMrndr
	./PPMrndr
