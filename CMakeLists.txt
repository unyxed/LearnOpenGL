cmake_minimum_required(VERSION 3.20)
set(CMAKE_MAKE_PROGRAM "C:/Program Files/Ninja/ninja.exe")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Fetch GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Fetch GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# Build GLAD as a static library
add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Build stb_image as a static library
add_library(stb_image STATIC external/stb/stb_image.cpp)
target_include_directories(stb_image PUBLIC external/stb)

# Helper function to define targets cleanly
function(add_gl_target target_name source_file)
    add_executable(${target_name} ${source_file})

    # Linked compiled libraries
    target_link_libraries(${target_name} PRIVATE
        glfw
        glad
        glm
        stb_image
        opengl32
    )

    # Tell the compiler where to find headers
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_BINARY_DIR}/_deps/glm-src
        ${CMAKE_SOURCE_DIR}/include
    )
endfunction()

# Define your executables
add_gl_target(LearnOpenGL src/main.cpp)
add_gl_target(Cube src/1_GettingStarted/cube.cpp)
add_gl_target(Flat_3D src/1_GettingStarted/flat_3d.cpp)
add_gl_target(Colors src/2_Lighting/colors.cpp)
