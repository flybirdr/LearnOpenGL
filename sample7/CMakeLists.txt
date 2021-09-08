cmake_minimum_required(VERSION 3.0.0)
project(sample1 VERSION 0.1.0)

# add_subdirectory(../glm/)
set(glm_DIR ../glm)
find_package(glm REQUIRED)

include(CTest)
enable_testing()


set(CMAKE_CXX_FLAGS "-std=c++11" )

add_executable(sample1 main.cpp stb_image.cpp shader.cpp)

#-lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
target_link_libraries(sample1
GLEW glfw GL X11 pthread Xrandr Xi glm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
