cmake_minimum_required(VERSION 2.8)

project(ambisonix)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)
file(GLOB in_hdrs include/ambisonix/*.h)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_library(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
    ${in_hdrs}
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

install(
    FILES ${hdrs}
    DESTINATION include
)

install(
    FILES ${in_hdrs}
    DESTINATION include/ambisonix
)
