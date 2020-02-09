cmake_minimum_required(VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)
project(matrixinator)

set (VERSION_MAJOR 1)
set (VERSION_MINOR 0)

set(BOOST_ROOT "E:/boost_1_72_0")
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Debug)

include_directories(
  ${PROJECT_SOURCE_DIR}/headers
  C:/Users/Leo/Desktop/VSCPP/libs
)

set(SOURCE
  ${PROJECT_SOURCE_DIR}/src/matrixinator.cpp
  ${PROJECT_SOURCE_DIR}/src/mtx.cpp
  C:/Users/Leo/Desktop/VSCPP/libs/utils.cpp
)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "E:/boost_1_72_0/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "E:/boost_1_72_0/boost/lib")

find_package(Boost)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
  add_executable(${PROJECT_NAME} ${SOURCE})
  target_link_libraries(${PROJECT_NAME} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY} ${Boost_SYSTEM_LIBRARY})
endif()
