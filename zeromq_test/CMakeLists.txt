cmake_minimum_required(VERSION 3.10)
project(zeromq_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Integrate vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

# Manually set CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};D:/Githhub_Repos/Prometheus_Exporter_for_ZeroMQ/vcpkg/installed/x64-mingw-dynamic")

# Find packages
find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

# Add the executables
add_executable(publisher publisher.cpp)
add_executable(subscriber subscriber.cpp)

# Link libraries
target_link_libraries(publisher PRIVATE libzmq cppzmq)
target_link_libraries(subscriber PRIVATE libzmq cppzmq)

# Include directories
target_include_directories(publisher PRIVATE ${ZeroMQ_INCLUDE_DIRS})
target_include_directories(subscriber PRIVATE ${ZeroMQ_INCLUDE_DIRS})