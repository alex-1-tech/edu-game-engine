# -------------------------
# Interface target for the whole engine
# Provides unified include and link interface
# -------------------------
add_library(engine_core INTERFACE)
add_library(EduGameEngine::core ALIAS engine_core)

# -------------------------
# Add core subdirectory which contains engine modules
# -------------------------
add_subdirectory(core)

# -------------------------
# Link dependencies to the interface target
# All targets linking to engine_core will get these automatically
# -------------------------
target_link_libraries(engine_core INTERFACE core)
target_link_libraries(engine_core INTERFACE SDL2-static)
target_link_libraries(engine_core INTERFACE engine_glm)

# -------------------------
# Platform-specific definitions
# Required for Windows to handle SDL's main entry point
# -------------------------
target_compile_definitions(engine_core INTERFACE
    $<$<PLATFORM_ID:Windows>:SDL_MAIN_HANDLED>
)

add_executable(engine_demo
    ${CMAKE_SOURCE_DIR}/engine/src/main.cpp
)

target_link_libraries(engine_demo PRIVATE engine_core)

# -------------------------
# Output configuration
# All executables will be placed in /bin directory
# -------------------------
set_target_properties(engine_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

message(STATUS "Building engine demo application")

# -------------------------
# Generate compile_commands.json for tools like clangd
# Available in CMake 3.13+
# -------------------------
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
