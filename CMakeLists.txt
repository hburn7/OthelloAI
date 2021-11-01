cmake_minimum_required(VERSION 3.17)
project(OthelloProject_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# All .cpp files must be included here.
add_executable(OthelloProject_cpp Core/Main.cpp Core/Logger.cpp Core/Utils.cpp
        Core/OthelloGame/BitBoard.cpp Core/OthelloGame/OthelloGameBoard.cpp
        Core/IO/Input/InputHandler.cpp Core/IO/Output/OutputHandler.cpp
        Core/Config.cpp Core/Config.h Core/OthelloGame/Color.h Core/OthelloGame/Move.cpp Core/OthelloGame/Move.h)