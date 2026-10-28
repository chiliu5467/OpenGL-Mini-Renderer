cmake_minimum_required(VERSION 3.20)

project(OpenGLMiniRenderer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and build GLFW
include(FetchContent)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# Build GLAD
add_library(glad STATIC
    external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include
)

# Build our application
add_executable(opengl_mini_renderer
    src/main.cpp
)

target_link_libraries(opengl_mini_renderer
    PRIVATE
        glad
        glfw
)

if(MSVC)
    target_compile_options(opengl_mini_renderer PRIVATE /utf-8)
    target_compile_options(glfw PRIVATE /utf-8)
endif()

# Define shader directory for the application
target_compile_definitions(opengl_mini_renderer PRIVATE
    SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders"
)