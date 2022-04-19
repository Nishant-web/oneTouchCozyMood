add_library(MP3_HANDLER STATIC mp3_handler.cpp)
target_include_directories(MP3_HANDLER PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Inc)
target_link_libraries(MP3_HANDLER PUBLIC wiringPi mpg123 pthread ao)
