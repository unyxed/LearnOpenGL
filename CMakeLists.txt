cmake_minimum_required(VERSION 3.20)
project(LearnOpenGL)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# system packages: sudo pacman -S base-devel cmake ninja glfw glm mesa glad stb
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# glad loader generated from the system glad tool into the build dir (fresh build dir regenerates it)
find_program(GLAD_TOOL glad)
if(NOT GLAD_TOOL)
    message(FATAL_ERROR "glad tool not found (sudo pacman -S glad)")
endif()
set(GLAD_OUT ${CMAKE_BINARY_DIR}/generated)
if(NOT EXISTS ${GLAD_OUT}/include/glad/gl.h)
    execute_process(
        COMMAND ${GLAD_TOOL} --api gl:core=3.3 --out-path ${GLAD_OUT} --reproducible --quiet c --loader
        RESULT_VARIABLE GLAD_RESULT
    )
    if(NOT GLAD_RESULT EQUAL 0)
        message(FATAL_ERROR "glad generation failed")
    endif()
endif()
add_library(glad STATIC ${GLAD_OUT}/src/gl.c)
target_include_directories(glad PUBLIC ${GLAD_OUT}/include)

# one exe per lesson; shared code (camera, stb impl) compiles straight in, no extra libs
function(add_gl_target target_name source_file)
    add_executable(${target_name} ${source_file}
        ${CMAKE_SOURCE_DIR}/src/common/camera.cpp
        ${CMAKE_SOURCE_DIR}/src/common/stb_impl.cpp
    )
    target_link_libraries(${target_name} PRIVATE glfw glad glm::glm OpenGL::GL)
    target_include_directories(${target_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    # absolute resources path so exes run from any working directory:
    # Shader(RESOURCE_DIR "shaders/x.vert", ...) concatenates two literals
    file(TO_CMAKE_PATH "${CMAKE_SOURCE_DIR}/resources" _resources_dir)
    target_compile_definitions(${target_name} PRIVATE "RESOURCE_DIR=\"${_resources_dir}/\"")
    if(DEFINED CHAPTER_NAME)
        set_target_properties(${target_name} PROPERTIES
            OUTPUT_NAME ${target_name}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CHAPTER_NAME}
        )
    else()
        set_target_properties(${target_name} PROPERTIES
            OUTPUT_NAME ${target_name}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
    endif()
endfunction()

#add_gl_target(LearnOpenGL src/main.cpp)

add_subdirectory(src/1-getting-started)
#add_subdirectory(src/2-lighting)
#add_subdirectory(src/3-model-loading)
#add_subdirectory(src/4-advanced-opengl)
#add_subdirectory(src/5-advanced-lighting)
#add_subdirectory(src/6-pbr)
