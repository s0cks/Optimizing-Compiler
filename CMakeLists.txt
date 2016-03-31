cmake_minimum_required(VERSION 3.3)
project(FlowGraph)

include_directories(Sources)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(FlowGraph main.cc Sources/ast.cc Sources/ast.h Sources/scope.cc Sources/scope.h Sources/type.cc Sources/type.h Sources/il.cc Sources/il.h Sources/flow_graph.cc Sources/flow_graph.h)