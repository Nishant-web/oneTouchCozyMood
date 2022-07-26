cmake_minimum_required(VERSION 3.15.4)
project(ENROLL_USERS)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")


add_compile_options(-Wall)
add_executable(ENROLL ${SOURCES})

find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
target_link_libraries(ENROLL ${WIRINGPI_LIBRARIES})


