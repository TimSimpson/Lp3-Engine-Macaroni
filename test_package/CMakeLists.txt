project(MyHello)
cmake_minimum_required(VERSION 2.8.12)

find_package(OpenGL REQUIRED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(${CONAN_INCLUDE_DIRS})

add_executable(hello hello.cpp)
target_link_libraries(hello ${OPENGL_LIBRARIES} ${CONAN_LIBS})
