cmake_minimum_required(VERSION 3.18.4)
project(MUSIC_PLAYER)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_compile_options(-Wall)
add_executable(PLAY ${SOURCES})

find_library(ALL_LIBRARIES NAMES wiringPi pthread mpg123 ao)
target_link_libraries(PLAY ${ALL_LIBRARIES})
