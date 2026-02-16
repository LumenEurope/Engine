cmake_minimum_required(VERSION 3.25)
project(mygame LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Download CPM if not present
file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CPM.cmake
    EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/CPM.cmake)

# Fetch SDL3 with Vulkan enabled
CPMAddPackage(
    NAME SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.0
    OPTIONS
        "SDL_VULKAN ON"
        "SDL_OPENGL OFF"
        "SDL_TEST OFF"
        "SDL_EXAMPLES OFF"
        "SDL_SHARED ON"
    EXCLUDE_FROM_ALL ON
)

# Find Vulkan from system SDK
find_package(Vulkan REQUIRED)

# Source files
set(SOURCES source/main.cpp)

add_executable(mygame ${SOURCES})

target_link_libraries(mygame PRIVATE
    SDL3::SDL3
    Vulkan::Vulkan
)

# Compiler features and warnings
target_compile_features(mygame PRIVATE cxx_std_20)

if (MSVC)
    target_compile_options(mygame PRIVATE /W4 /WX)
else()
    target_compile_options(mygame PRIVATE -Wall -Wextra -Wpedantic)
endif()
