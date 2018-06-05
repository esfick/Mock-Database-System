FLAGS = -std=c++14
#FLAGS = -Wall -std=c++14 -g -O0 -pedantic-errors
CPPFLAGS = -I /Users/emmafick/boost_1_67_0
all: MyDb

MyDb: main.o Server.o run_server.o run_database.o run_sql.o
	g++ $(FLAGS) -o MyDb main.o Server.o run_server.o run_database.o run_sql.o
main.o: main.cpp Functions.h Server.h
	g++ $(FLAGS) $(CPPFLAGS) -c main.cpp

Server.o: Server.cpp Server.h
	g++ $(FLAGS) $(CPPFLAGS) -c Server.cpp

run_server.o: run_server.cpp functions.h Server.h
	g++ $(FLAGS) $(CPPFLAGS) -c run_server.cpp

run_database.o: run_database.cpp functions.h Server.h
	g++ $(FLAGS) $(CPPFLAGS) -c run_database.cpp

run_sql.o: run_sql.cpp functions.h Server.h
	g++ $(FLAGS) $(CPPFLAGS) -c run_sql.cpp

clean:
	rm -f MyDb
	rm -f *.o
