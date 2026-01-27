cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(glfw_app)
find_package(OpenGL REQUIRED)

add_subdirectory( glfw3 )
add_executable(glfw_app main.cpp glad.c )

target_include_directories(glfw_app
    PRIVATE
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(glfw_app ${OPENGL_LIBRARIES} glfw )