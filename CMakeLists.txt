cmake_minimum_required(VERSION 3.14)

project(classify LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")
set(KOCCHI_BINARY_DIR ${CMAKE_BINARY_DIR}/kocchi-classify)

set(venv_name venv)


# find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets REQUIRED)
find_package(Eigen3 3.3 REQUIRED)
find_package(LDA++ REQUIRED)

# find_package(Python COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

# Create the virtual environment
execute_process(
    COMMAND python -m venv ${KOCCHI_BINARY_DIR}/${venv_name}
    RESULT_VARIABLE venv_result
)

if(venv_result)
    message(FATAL_ERROR "Failed to create virtual environment")
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} -E env "PATH=${KOCCHI_BINARY_DIR}/${venv_name}/bin:$ENV{PATH}" pip install scikit-learn
    RESULT_VARIABLE pip_result
)

if(pip_result)
    message(FATAL_ERROR "Failed to install scikit-learn")
endif()

file(MAKE_DIRECTORY ${KOCCHI_BINARY_DIR}/modules/)
message(STATUS "Copying ${PROJECT_SOURCE_DIR}/modules/tokenize.py")

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/modules/tokenize.py ${KOCCHI_BINARY_DIR}/modules/tokenize.py
    RESULT_VARIABLE copy_result
)

if(copy_result)
    message(FATAL_ERROR "Failed to copy file ${PROJECT_SOURCE_DIR}/modules/tokenize.py > ${KOCCHI_BINARY_DIR}/modules/tokenize.py : ${copy_result}")
endif()

# Bring the headers into the project
include_directories(
    ${EIGEN_INCLUDE_DIRS}
    ${LDA_INCLUDE_DIRS}
    ${PYBIND_INCLUDE_DIRS}
)

add_executable(classify
  main.cpp
  pkg/Common.cpp
)


set_target_properties(classify PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${KOCCHI_BINARY_DIR}/bin
)

add_custom_target(CopyFiles DEPENDS ${KOCCHI_BINARY_DIR}/modules/tokenize.py)
target_link_libraries(classify Qt5::Core Qt5::Widgets)
add_dependencies(classify CopyFiles)