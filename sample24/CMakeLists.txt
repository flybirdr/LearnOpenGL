cmake_minimum_required(VERSION 3.0.0)
project(sample1 VERSION 0.1.0)

# add_subdirectory(../glm/)
set(glm_DIR ../glm)
find_package(glm REQUIRED)

add_definitions(-DCWD="${CMAKE_SOURCE_DIR}";)


include(CTest)
enable_testing()


set(CMAKE_CXX_FLAGS "-std=c++11" )

add_executable(sample1 main.cpp stb_image.cpp)

#-lglfw -lGL -lX11 -lpthread -lXrandr -lXi
target_link_libraries(sample1
 glfw GL X11 pthread Xrandr Xi glm glad dl assimp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
