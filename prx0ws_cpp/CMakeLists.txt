     
project(test_websocket_echoback CXX)
cmake_minimum_required(VERSION 3.12)

find_package(cppzmq REQUIRED)

add_executable(prx0ws prx0ws.cc)
target_include_directories(prx0ws PUBLIC ${cppzmq_INCLUDE_DIR})
target_link_libraries(prx0ws cppzmq-static )
