cmake_minimum_required(VERSION 3.1)
project(testTask_opencv)
set(TARGET ${PROJECT_NAME})

find_package(OpenCV REQUIRED)

SET(SRC src/main.cpp)

add_executable(${TARGET} ${SRC})

target_link_libraries(${TARGET} PRIVATE ${OpenCV_LIBS})