﻿# CMakeList.txt: Raytracing-in-one-week 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

include_directories(./include)
# 将源代码添加到此项目的可执行文件。
add_executable (Raytracing "include/vec3.h" "include/color.h" "include/ray.h" 
"include/hittable.h" "include/sphere.h" "include/hittable_list.h"  
"include/rtweekend.h" "main.cpp"   "include/camera.h" "include/material.h")



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Raytracing PROPERTY CXX_STANDARD 20)
endif()


# TODO: 如有需要，请添加测试并安装目标。
