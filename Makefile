main: main.o Game.o
	g++ -o main main.o Game.o

main.o: main.cpp
	g++ -c main.cpp

Game.o: Game.cpp
	g++ -c Game.cpp
