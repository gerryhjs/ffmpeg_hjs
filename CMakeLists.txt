cmake_minimum_required(VERSION 2.6)
project(ffmpeg_test LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 不需要RTSP摘要认证就删除-DAUTH_CONFIG
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DXOP_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(EXECUTABLE_OUTPUT_PATH ../)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})



set(CODING_SOURCES
        ${PROJECT_SOURCE_DIR}/VideoEncoding.cpp
        ${PROJECT_SOURCE_DIR}/VideoDecoding.cpp
        )
add_library(coding ${CODING_SOURCES})

#add_executable(rtsp_server ${PROJECT_SOURCE_DIR}/example/rtsp_server.cpp)
#target_link_libraries(rtsp_server xop net pthread)
#
#add_executable(rtsp_pusher ${PROJECT_SOURCE_DIR}/example/rtsp_pusher.cpp)
#target_link_libraries(rtsp_pusher xop net pthread)


include_directories(${PROJECT_SOURCE_DIR}/out/include)
link_directories(${PROJECT_SOURCE_DIR}/out/lib)

add_executable(ffmpeg_test ${PROJECT_SOURCE_DIR}/main.cpp)
target_link_libraries(ffmpeg_test coding
        avcodec
        avdevice
        avfilter
        avformat
        avutil
        swresample
        swscale
        )
