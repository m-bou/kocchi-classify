cmake_minimum_required(VERSION 3.14)

project(classify LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

# find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(LDA++ REQUIRED)

# find_package(Docopt REQUIRED)
# find_package(Eigen REQUIRED)
# find_package(LDA++ REQUIRED)

# Bring the headers into the project
include_directories(
    # include
    ${EIGEN_INCLUDE_DIRS}
    ${LDA_INCLUDE_DIRS}
)

add_executable(classify
  main.cpp
)

# set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_target_properties(classify PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_link_libraries(classify Qt5::Core Qt5::Widgets Eigen3::Eigen)
