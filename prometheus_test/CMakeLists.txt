cmake_minimum_required(VERSION 3.10)
project(prometheus_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Integrate vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

find_package(prometheus-cpp CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(civetweb CONFIG REQUIRED)

add_executable(prometheus_test main.cpp)

target_link_libraries(prometheus_test
  PRIVATE
    prometheus-cpp::core
    prometheus-cpp::pull
    civetweb::civetweb
    ZLIB::ZLIB
)