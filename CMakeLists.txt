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

# rknn runtime
# for rknpu2
set(RKNN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/rknpu2)
set(LIBRKNNRT ${RKNN_PATH}/${CMAKE_SYSTEM_NAME}/aarch64/librknnrt.so)
set(LIBRKNNRT_INCLUDES ${RKNN_PATH}/include)
message("RKNN lib: " ${LIBRKNNRT})
message("RKNN include: " ${LIBRKNNRT_INCLUDES})
include_directories(/usr/include/rga/ /usr/include/stb/)

set(UTILS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/utils)
add_subdirectory(utils utils.out)

include_directories(yolov5/cpp ${LIBRKNNRT_INCLUDES} ${UTILS_INCLUDES})

add_subdirectory(yolov5/cpp)

add_executable(test 
    test.cpp
    yolov5/cpp/postprocess.cc
    yolov5/cpp/rknpu2/yolov5.cc
)
target_link_libraries(test 
    ${GST_LIBRARIES} 
    ${OpenCV_LIBS}
    imageutils
    fileutils
    imagedrawing
    ${LIBRKNNRT}
    dl
)
