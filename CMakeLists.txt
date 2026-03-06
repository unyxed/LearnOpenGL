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