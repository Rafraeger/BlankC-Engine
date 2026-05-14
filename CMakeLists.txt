cmake_minimum_required(VERSION 3.16)
project("BlankC Engine" VERSION 0.1.0 LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ========================
# Dependencies
# ========================

# GLFW via FetchContent
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glfw)

# GLAD - single source file (libs/glad/src/glad.c)
# stb_image - header-only (libs/stb/stb_image.h)

# ========================
# Engine Library
# ========================

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/physics)
include_directories(${CMAKE_SOURCE_DIR}/include/ui)
include_directories(${CMAKE_SOURCE_DIR}/include/audio)
include_directories(${CMAKE_SOURCE_DIR}/include/interaction)
include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/stb)
include_directories(${CMAKE_SOURCE_DIR}/libs/nuklear)
include_directories(${CMAKE_SOURCE_DIR}/libs/miniaudio)

# Source files
set(ENGINE_SOURCES
    src/engine.c
    src/core/log.c
    src/core/math.c
    src/core/memory.c
    src/core/time.c
    src/rendering/gl_context.c
    src/rendering/shader.c
    src/rendering/mesh.c
    src/rendering/camera.c
    src/rendering/renderer.c
    src/rendering/texture.c
    src/input/input.c
    src/scene/transform.c
    src/scene/scenegraph.c
    src/resource/loader.c
    src/resource/obj_loader.c
    src/physics/physics.c
    src/ui/ui.c
    src/audio/audio.c
    src/interaction/interaction.c
    src/camera_controller.c
    libs/glad/src/glad.c
)




# Create static library
add_library(enginec STATIC ${ENGINE_SOURCES})

# Link dependencies
target_link_libraries(enginec PRIVATE glfw)

# Platform-specific linking
if(UNIX AND NOT APPLE)
    target_link_libraries(enginec PRIVATE dl)
endif()

# Compiler definitions per platform
if(WIN32)
    target_compile_definitions(enginec PRIVATE _WIN32 ENGINE_PLATFORM_WINDOWS NOGDI)
elseif(UNIX AND NOT APPLE)
    target_compile_definitions(enginec PRIVATE __linux__ ENGINE_PLATFORM_LINUX)
elseif(APPLE)
    target_compile_definitions(enginec PRIVATE __APPLE__ ENGINE_PLATFORM_MACOS)
endif()

# ========================
# Examples
# ========================

add_executable(examples_triangle examples/triangle.c)
target_link_libraries(examples_triangle PRIVATE enginec glfw)

add_executable(examples_quad examples/quad.c)
target_link_libraries(examples_quad PRIVATE enginec glfw)

add_executable(examples_shader_test examples/shader_test.c)
target_link_libraries(examples_shader_test PRIVATE enginec glfw)

add_executable(examples_obj_demo examples/obj_demo.c)
target_link_libraries(examples_obj_demo PRIVATE enginec glfw)

add_executable(examples_demo examples/demo.c)
target_link_libraries(examples_demo PRIVATE enginec glfw)

add_executable(fps_game src/fps_game.c)
target_link_libraries(fps_game PRIVATE enginec glfw)
