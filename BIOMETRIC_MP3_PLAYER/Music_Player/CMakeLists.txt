cmake_minimum_required(VERSION 3.15.4)
project(MUSIC_PLAYER)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_compile_options(-Wall)
add_executable(PLAY ${SOURCES})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lao -lmpg123 -lfltk -lwiringPi")
find_library(ALL_LIBRARIES NAMES wiringPi pthread mpg123 ao fltk)
target_link_libraries(PLAY ${ALL_LIBRARIES})
