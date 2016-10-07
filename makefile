#
# This is the makefile for popstar solver. The solver contains two modules
# PopStarSolver is a command line module that takes input or creates data to solve.
# Solver is the module that does the actual work. It includes a board model and soloution algorithms
# as well as some supporting code (thread pools, concurent data etc)

#PopStarSolver 
#   - PopStarSolver.cpp: contains the main() function which calls the solver. Currently there are 
# only pre-set options for testing. Eventually we'll need input.

#Solver
#   - PopStarBoard.cpp/PopStarBoard.h: Model for the popstar board. Includes both data and behavior 
# (e.g. Popping, collapsing etc)

#   - PuzzleSolution.cpp/PuzzleSolution.h: Defines two classes. an actual solver class that does the 
#solving and a solution class that defines possible solutions (including partial ones)

#   - Solver.cpp/Solver.h: This class supplies the interface to the shared module. Right now it only
# has some predefined boards (including random), but eventually it will need to be supplied a board.

#   - Stdafx.cpp/stdafx.h/targetver.h: standard boilerplace required for VS windows build.

#   - Threadpool.cpp/Threadpool.h/Threadpool.tpp: contains a pool of worker thread. the .cpp is not
# actually used it's just an artifact of a previous set of code. The thread is seperated into a .tpp 
# .h so the entirety of the code can be defined in the .h file (simple include to get all the 
# funtionality)

#   - ConcurrentQueu.h: A couple of templated collections with a mutex to store input and output 
# between the worker threads 


#Variables
# this control whether or not we're doing an import or export
CC = g++
DEBUG = -g
CFLAGS = -c -Wall -std=c++17 -fPIC
LFLAGS = -Wall
SFLAGS = -shared
TARGET = SolveBoard
TARGETDIR = ./obj/
INCLUDEDIR = -I./Solver -I./PopStarSolver

PopStarSolver: Solver.so
	$(CC) -g $(INCLUDEDIR) -pthread -oPopStarSolver.out ./PopStarSolver/PopStarSolver.cpp $(TARGETDIR)Solver.so

Solver.so: PuzzleSolution.o PopStarBoard.o Solver.o
	$(CC) $(SFLAGS) $(INCLUDEDIR) -o$(TARGETDIR)Solver.so $(TARGETDIR)PuzzleSolution.o $(TARGETDIR)PopStarBoard.o $(TARGETDIR)Solver.o

PuzzleSolution.o: ./Solver/ThreadPool.tpp
	$(CC) $(INCLUDEDIR) $(CFLAGS) ./Solver/PuzzleSolution.cpp -o $(TARGETDIR)PuzzleSolution.o

PopStarBoard.o:
	$(CC) $(INCLUDEDIR) $(CFLAGS) ./Solver/PopStarBoard.cpp -o $(TARGETDIR)PopStarBoard.o

Solver.o:
	$(CC) $(INCLUDEDIR) $(CFLAGS) ./Solver/Solver.cpp -o $(TARGETDIR)Solver.o

#debug: #degug

clean: 
	rm $(TARGETDIR)*.* ./PopStarSolver.out
#add the intermediate files