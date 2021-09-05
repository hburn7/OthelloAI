cmake_minimum_required(VERSION 3.17)
project(OthelloProject_cpp)

set(CMAKE_CXX_STANDARD 20)

# All .cpp files must be included here.
add_executable(OthelloProject_cpp Core/Main.cpp Core/Logger.cpp
        Core/OthelloGame/BitBoard.cpp Core/OthelloGame/OthelloGameBoard.cpp Core/OthelloGame/GameState.cpp
        Core/IO/Input/InputHandler.cpp Core/IO/Output/OutputHandler.cpp
        Core/Agent/Agent.cpp
)