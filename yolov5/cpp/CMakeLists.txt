cmake_minimum_required(VERSION 3.10)

project(rknn_yolov5_demo)

set(rknpu_yolov5_file rknpu2/yolov5.cc)

# rknn runtime
# for rknpu2
set(RKNN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../rknpu2)
set(LIBRKNNRT ${RKNN_PATH}/${CMAKE_SYSTEM_NAME}/aarch64/librknnrt.so)
set(LIBRKNNRT_INCLUDES ${RKNN_PATH}/include)
message("RKNN lib: " ${LIBRKNNRT})
message("RKNN include: " ${LIBRKNNRT_INCLUDES})

include_directories(/usr/include/rga/ /usr/include/stb/)

set(UTILS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../../utils)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../utils utils.out)

add_executable(${PROJECT_NAME}
    main.cc
    postprocess.cc
    ${rknpu_yolov5_file}
)
target_link_libraries(${PROJECT_NAME}
    imageutils
    fileutils
    imagedrawing
    ${LIBRKNNRT}
    dl
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBRKNNRT_INCLUDES}
    ${UTILS_INCLUDES}
)
