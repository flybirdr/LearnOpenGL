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

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

#mac os 需要使用brew 按照以下依赖
if (APPLE)
#brew install glew
#brew install glfw
#brew install assimp
#brew install freetype

#头文件
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/glew/include")
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/glfw/include")
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/assimp/include")
include_directories(BEFORE SYSTEM "/opt/homebrew/opt/freetype2/include/freetype2")
#链接库
link_directories(/opt/homebrew/opt/glew/lib)
link_directories(/opt/homebrew/opt/glfw/lib)
link_directories(/opt/homebrew/opt/assimp/lib)
link_directories(/opt/homebrew/opt/freetype2/lib)
#所有的target指定link库
link_libraries(GLEW glfw glm assimp freetype)
#设置linker flags
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo")
message(">>> exe_link_flags=${CMAKE_EXE_LINKER_FLAGS}")
endif()

if(LINUX)
#pacman -S glfw
#pacman -S glew
#pacman -S freetype
#pacman -S assimp

#不需要手动link_directories，Linux包管理器会自动安装到系统include和lib

#所有的target指定link库
link_libraries(glfw GL X11 pthread Xrandr Xi glm glad dl assimp)

endif(LINUX)

#打印一下，看看include和lib目录是否正确导入
get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(">>> include_dirs=${dirs}") #打印一下目录情况
get_property(dirs DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY LINK_DIRECTORIES)
message(">>> link_dirs=${dirs}") #打印一下目录情况

#glad的依赖
file(GLOB GLAD "${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.c")
message(">>> glad src=${GLAD}")

#罗列所有sample文件夹
file(GLOB SRCS "${CMAKE_CURRENT_SOURCE_DIR}/sample*")
# message(">>> SRCS=${SRCS}")

set(NUM 1)
foreach(src ${SRCS})
    #计数+1
    math(EXPR NUM "${NUM} + 1")

    #罗列所有源文件
    file(GLOB SAMPLE${NUM}_SRC "./sample${NUM}/*.cpp")
    #target名字
    set(TMP_BUILD_TARGET sample${NUM})

    #源文件为空则不添加target
    if(SAMPLE${NUM}_SRC STREQUAL "")
        continue()
    endif()
    #添加Target
    add_executable(${TMP_BUILD_TARGET} ${SAMPLE${NUM}_SRC} ${GLAD})

    message(">>> Target  name = ${TMP_BUILD_TARGET} ")
    message(">>>         srcs = ${SAMPLE${NUM}_SRC} ")
endforeach(src)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)




