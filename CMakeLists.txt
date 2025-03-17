cmake_minimum_required(VERSION 3.30)
project(pingpong
	VERSION 0.1.0
	DESCRIPTION "A TCP ping pong console app"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pingpong src/main.cpp)
