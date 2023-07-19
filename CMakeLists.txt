cmake_minimum_required(VERSION 3.0.0)
project(sample1 VERSION 0.1.0)

set(glm_DIR "${CMAKE_CURRENT_SOURCE_DIR}/glm")
find_package(glm REQUIRED)
message(">>> glm_FOUND=${glm_FOUND}")

# 依赖项目内的assimp
# set(assimp_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assimp/generated")
# find_package(assimp REQUIRED)
# message(">>> assimp_FOUND=${assimp_FOUND}")
# message(">>> assimp_LIBRARIES=${assimp_LIBRARIES}")

set(CMAKE_CXX_FLAGS "-std=c++11" )

include(CTest)
enable_testing()

#本项目的glad依赖
include_directories(BEFORE SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/glad/include)
#mac os 需要使用brew 按照以下依赖
if(APPLE)

#brew install glew
#brew install glfw
#brew install assimp

include_directories(BEFORE SYSTEM "/opt/homebrew/opt/glew/include")
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/glfw/include")
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/assimp/include")

link_directories(/opt/homebrew/opt/glew/lib)
link_directories(/opt/homebrew/opt/glfw/lib)
link_directories(/opt/homebrew/opt/assimp/lib)
endif(APPLE)



set(EXECUTABLE_OUTPUT_PATH CMAKE_CURRENT_SOURCE_DIR)

if (APPLE)
link_libraries(GLEW glfw glm assimp)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
message(">>> exe_link_flags=${CMAKE_EXE_LINKER_FLAGS}")
endif()

if(LINUX)
link_libraries(glfw GL X11 pthread Xrandr Xi glm glad dl assimp)
endif(LINUX)

get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(">>> include_dirs=${dirs}") #打印一下目录情况
get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message(">>> link_dirs=${dirs}") #打印一下目录情况

file(GLOB GLAD "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c")
message(">>> glad src=${GLAD}")

file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/sample*")
# message(">>> SRCS=${SRCS}")

set(NUM 1)
foreach(src ${SRCS})
    #计数+1
    math(EXPR NUM "${NUM} + 1")

    
    file(GLOB SAMPLE${NUM}_SRC "./sample${NUM}/*.cpp")

    set(TMP_BUILD_TARGET sample${NUM})

    #源文件为空则不添加target
    if(SAMPLE${NUM}_SRC STREQUAL "")
        continue()
    endif(EMPTYP)
    #添加Target
    add_executable(${TMP_BUILD_TARGET} ${SAMPLE${NUM}_SRC} ${GLAD})

    message(">>> Target=${TMP_BUILD_TARGET} ")
    message(">>> Target srcs=${SAMPLE${NUM}_SRC} ")
endforeach(src)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




