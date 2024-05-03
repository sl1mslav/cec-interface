cmake_minimum_required(VERSION 3.22.1)

project("cec-interface")


add_library(
        cec
        SHARED
        IMPORTED
)

set_target_properties(
        cec
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}/libcec.so
)

add_library(
        ${CMAKE_PROJECT_NAME}
        SHARED
        CECInterface.cpp
)

target_include_directories(
        ${CMAKE_PROJECT_NAME}
        PRIVATE
        src/main/cpp/include/
)

target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        cec
        android
        log
)