cmake_minimum_required(VERSION 3.20)
set(CMAKE_MAKE_PROGRAM "C:/Program Files/Ninja/ninja.exe")

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(LearnOpenGL src/main.cpp)

target_link_libraries(LearnOpenGL
    PRIVATE
    glfw
    glad::glad
    glm::glm
		opengl32
)

add_executable(Cube src/1_GettingStarted/cube.cpp)

target_link_libraries(Cube
    PRIVATE
    glfw
    glad::glad
    glm::glm
		opengl32
)

add_executable(Flat_3D src/1_GettingStarted/flat_3d.cpp)

target_link_libraries(Flat_3D
    PRIVATE
    glfw
    glad::glad
    glm::glm
		opengl32
)

add_executable(Colors src/2_Lighting/colors.cpp)

target_link_libraries(Colors
    PRIVATE
    glfw
    glad::glad
    glm::glm
		opengl32
)