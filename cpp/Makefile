all: car road node city simulator main

main:
	g++ -O3 -std=c++17 -pthread -g -o main main.cpp car.o road.o node.o city.o simulator.o 

car:
	g++ -O3 -c -g -o car.o car.cpp

road:
	g++ -O3 -c -g -o road.o road.cpp

node:
	g++ -O3 -c -g -o node.o node.cpp

city:
	g++ -O3 -c -g -o city.o city.cpp

simulator:
	g++ -O3 -c -g -o simulator.o simulator.cpp 

clean:
	rm *.o main
