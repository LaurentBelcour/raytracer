cmake_minimum_required (VERSION 2.6)

project(Raytracer)

find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

add_executable(test main.cpp TriangleMesh.cpp)

target_link_libraries(test ${Threads_LIBS})
target_link_libraries(test ${X11_LIBS})

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm -lpthread -lX11")
SET(CMAKE_BUILD_TYPE "Debug")
