cmake_minimum_required(VERSION 3.16.0)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
list(APPEND EXTRA_COMPONENT_DIRS .pio/libdeps/esp32dev/esp32-camera )
#list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/esp32dev/esp-tflite-micro" )
#list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/esp32dev/esp-tflite-micro/third_party/gemmlowp/fixedpoint") 
#list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/esp32dev/esp-tflite-micro/third_party/flatbuffers") 
#list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/.pio/libdeps/esp32dev/esp-tflite-micro/third_party/flatbuffers/include/flatbuffers") 
project(esp32-cam)