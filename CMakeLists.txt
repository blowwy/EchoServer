cmake_minimum_required(VERSION 3.12)
project(EServer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -Wextra ")
#set(CMAKE_CXX_STANDARD_REQUIRED on)

#set(GTEST_USE_STATIC_LIBS OFF   )

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)е



include_directories(${GTEST_INCLUDE_DIRS})



add_executable(Server epoll.cpp epoll.h BadBehavior.cpp BadBehavior.h Server.cpp Server.h Socket.cpp Socket.h main_echo_server.cpp)

add_executable(Test test.cpp)
target_link_libraries(Test ${GTEST_LIBRARIES} Threads::Threads)

