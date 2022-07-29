cmake_minimum_required(VERSION 3.15.4)
project(UNIT_TEST)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_compile_options(-Wall)
add_executable(UNIT_TEST ${SOURCES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lao -lmpg123 -lfltk -lwiringPi -lgtest")
find_library(ALL_LIBRARIES NAMES wiringPi pthread mpg123 ao fltk gtest)
target_link_libraries(UNIT_TEST ${ALL_LIBRARIES})
