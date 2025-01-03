cmake_minimum_required(VERSION 3.10)
project(my_cpp_project)

set(CMAKE_CXX_STANDARD 17)

# Add the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")

# Set the CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "D:/Githhub_Repos/Prometheus_Exporter_for_ZeroMQ/vcpkg/installed/x64-mingw-static")

find_package(prometheus-cpp CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)

add_executable(src main.cpp metrics.cpp messaging.cpp)
target_link_libraries(src PRIVATE prometheus-cpp::core prometheus-cpp::pull cppzmq)

# Add receiver executable
add_executable(receiver receiver.cpp metrics.cpp)
target_link_libraries(receiver PRIVATE prometheus-cpp::core prometheus-cpp::pull cppzmq)