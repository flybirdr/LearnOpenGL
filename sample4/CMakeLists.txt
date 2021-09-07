cmake_minimum_required(VERSION 3.0.0)
project(sample1 VERSION 0.1.0)

find_package(glfw3 3.3 REQUIRED)

include(CTest)
enable_testing()


set(CMAKE_CXX_FLAGS "-std=c++11" )

add_executable(sample1 main.cpp)

#-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
target_link_libraries(sample1
GLEW glfw GL X11 pthread Xrandr Xi)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
