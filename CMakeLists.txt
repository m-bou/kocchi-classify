cmake_minimum_required(VERSION 3.14)

project(classify LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Eigen REQUIRED)
find_package(LDA++ REQUIRED)

# Bring the headers into the project
include_directories(
    include
    ${EIGEN_INCLUDE_DIRS}
)


add_executable(classify
  main.cpp
)
target_link_libraries(classify Qt${QT_VERSION_MAJOR}::Core)
