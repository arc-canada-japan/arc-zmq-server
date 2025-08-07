     
project(test_websocket_echoback)
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)

find_package(cppzmq REQUIRED)
#find_package(yaml-cpp REQUIRED)

find_package(PkgConfig)
pkg_check_modules(YAMLCPP REQUIRED yaml-cpp)
message(STATUS "yaml-cpp library: ${YAMLCPP_LINK_LIBRARIES}")

add_executable(sideA sideA.cpp sha256.cpp)
set_target_properties( sideA PROPERTIES COMPILE_FLAGS "-DUSE_DUMP" )
target_include_directories(sideA PUBLIC ${cppzmq_INCLUDE_DIR})
#target_link_directories()
target_link_libraries(sideA cppzmq-static ${YAMLCPP_LINK_LIBRARIES})

add_executable(sideB sideB.cpp)
target_include_directories(sideB PUBLIC ${cppzmq_INCLUDE_DIR})
target_link_libraries(sideB cppzmq-static ${YAMLCPP_LINK_LIBRARIES})


