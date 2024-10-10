cmake_minimum_required(VERSION 3.10)

# Project name
project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find the GStreamer package
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-app-1.0)
# Include GStreamer headers
include_directories(${GST_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)

add_subdirectory(yolov5/cpp)

# Add executable
add_executable(test test.cpp)

# Link GStreamer libraries
target_link_libraries(test ${GST_LIBRARIES} ${OpenCV_LIBS})
