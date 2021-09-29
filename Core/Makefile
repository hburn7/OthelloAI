C_FILES = Main.cpp Logger.cpp Utils.cpp Agent/Agent.cpp IO/Input/InputHandler.cpp IO/Output/OutputHandler.cpp OthelloGame/BitBoard.cpp OthelloGame/OthelloGameBoard.cpp
C_FLAGS = -std=c++17 -O -I ./ -I ./Agent -I ./IO -I ./IO/Input -I ./IO/Output -I ./OthelloGame
PROGRAM = hburnet2Othello

Main.o: Main.cpp
	g++ $(C_FLAGS) -c Main.cpp -lm

Logger.o: Logger.cpp
	g++ $(C_FLAGS) -c Logger.cpp

Utils.o: Utils.cpp
	g++ $(C_FLAGS) -c Utils.cpp

Agent.o: Agent/Agent.cpp
	g++ $(C_FLAGS) -c Agent/Agent.cpp

InputHandler.o: IO/Input/InputHandler.cpp
	g++ $(C_FLAGS) -c IO/Input/InputHandler.cpp

OutputHandler.o: IO/Output/OutputHandler.cpp
	g++ $(C_FLAGS) -c IO/Output/OutputHandler.cpp

BitBoard.o: OthelloGame/BitBoard.cpp
	g++ $(C_FLAGS) -c OthelloGame/BitBoard.cpp

OthelloGameBoard.o: OthelloGame/OthelloGameBoard.cpp
	g++ $(C_FLAGS) -c OthelloGame/OthelloGameBoard.cpp

OBJECTS = Main.o Logger.o Utils.o Agent.o InputHandler.o OutputHandler.o BitBoard.o OthelloGameBoard.o

all: $(PROGRAM)
$(PROGRAM):$(OBJECTS)
	g++ $(OBJECTS) -lm -o hburnet2Othello.out
	rm *.o
