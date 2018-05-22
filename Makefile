FLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors

all: MyDb

MyDb: main.o Server.o run_server.o run_database.o
	g++ $(FLAGS) -o MyDb main.o Server.o run_server.o run_database.o

main.o: main.cpp Functions.h Server.h
	g++ $(FLAGS) -c main.cpp

Server.o: Server.cpp Server.h
	g++ $(FLAGS) -c Server.cpp

run_server.o: run_server.cpp functions.h Server.h
	g++ $(FLAGS) -c run_server.cpp

run_database.o: run_database.cpp functions.h Server.h
	g++ $(FLAGS) -c run_database.cpp

clean:
	rm -f MyDb
	rm -f *.o
