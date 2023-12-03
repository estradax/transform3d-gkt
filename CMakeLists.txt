cmake_minimum_required(VERSION 3.20)
project(transform3d-gkt CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(freeglut_directory third_party/freeglut-3.4.0)

set(FREEGLUT_BUILD_DEMOS False)
set(OpenGL_GL_PREFERENCE LEGACY)
add_subdirectory(${freeglut_directory})

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${freeglut_directory}/include)
target_link_libraries(${PROJECT_NAME} freeglut GLU)
