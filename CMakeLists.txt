cmake_minimum_required(VERSION 3.0.0)
project(PLatformLinux VERSION 0.1.0)

set(PROJECT_SOURCE_DIR "/home/choisw1019/ProjectCPP/PLatformLinux")
set(SOURCE main.cpp)

MESSAGE(include_directories("/home/choisw1019/ProjectCPP/PlatformLinux/"))
add_executable(${PROJECT_NAME} ${SOURCE})
