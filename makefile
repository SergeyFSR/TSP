output: main.o tsp.o
	g++ main.o TSP.o -o output

main.o: main.cpp
	g++ -c -std=c++17  main.cpp 

tsp.o: tsp.cpp tsp.hpp
	g++ -c -std=c++17  tsp.cpp

clean:
	rm *.o output
