cmake_minimum_required(VERSION 3.17)
project(ass1)

set(CMAKE_CXX_STANDARD 11)

add_executable(ass1 src/main.cpp include/Session.h include/Graph.h include/Agent.h include/Tree.h src/Session.cpp src/Graph.cpp src/Agent.cpp src/Tree.cpp src/FiveRuleTester.cpp src/Tester.cpp)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o")