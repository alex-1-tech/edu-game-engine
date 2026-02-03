cmake_minimum_required(VERSION 3.20...3.30)
project(EduGameEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)

# -------------------------
# Fetch SDL2
# -------------------------
set(SDL2_DISABLE_INSTALL ON CACHE BOOL "" FORCE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
set(SDL_TEST OFF CACHE BOOL "" FORCE)
set(SDL2IMAGE_SHARED OFF CACHE BOOL "Build SDL2_image as static library" FORCE)
set(SDL2IMAGE_STATIC ON CACHE BOOL "Build SDL2_image as static library" FORCE)
set(SDL2IMAGE_VENDORED OFF CACHE BOOL "Use vendored libraries" FORCE)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG release-2.32.10
    SYSTEM
)

# -------------------------
# Fetch GLM
# -------------------------
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.0
    SYSTEM
)

# -------------------------
# Fetch lohmann/json
# -------------------------

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0 
)

# Fetch SDL2_image
# -------------------------

FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-2.8.8
    SYSTEM
)



FetchContent_MakeAvailable(SDL2 json glm SDL2_image)


# -------------------------
# GLM interface target
# -------------------------
add_library(engine_glm INTERFACE)
target_include_directories(engine_glm INTERFACE ${glm_SOURCE_DIR})
target_compile_definitions(engine_glm INTERFACE GLM_ENABLE_EXPERIMENTAL)

# -------------------------
# JSON interface target
# -------------------------
add_library(engine_json INTERFACE)
target_include_directories(engine_json INTERFACE ${json_SOURCE_DIR}/single_include)



add_subdirectory(engine)
# add_subdirectory(editor)
# add_subdirectory(python)
# add_subdirectory(tests)