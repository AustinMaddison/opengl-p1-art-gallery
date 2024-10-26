cmake_minimum_required(VERSION 3.0.0)
project(lab4 VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(lab4 
    src/config.h 
    src/config.cpp
    src/material.h
    src/material.cpp
    src/Lab4_textured_box.cpp 
    src/glad.c
)

target_include_directories(lab4 PRIVATE dependencies)

find_package(glfw3 3.4 REQUIRED)

target_link_libraries(lab4 glfw)